/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/** @file
 *
 * Reference code example for CapsenseSlider values published to AWS Cloud using AWS IoT publisher  
 */
#include "mbed.h"
#include "aws_client.h"
#include "aws_config.h"
#include "cy_pdl.h"
#include "cycfg_capsense.h"
#include "cycfg.h"
#include "mbed_events.h"

#define APP_INFO( x )                        printf x
#define AWSIOT_KEEPALIVE_TIMEOUT            (60)
#define AWSIOT_MESSAGE                      "Starting AWS MQTT Connection"
#define AWS_IOT_SECURE_PORT                 (8883)
#define AWSIOT_TIMEOUT                      (1000)
#define SLIDER_NUM_TOUCH                        (1u)    /* Number of touches on the slider */
#define LED_OFF                                 (1u) 
#define LED_ON                                  (0u)
#define CAPSENSE_SCAN_PERIOD                    (20u)   /* milliseconds */


/*****************************************************************************
* Class Name: AWSConnectionParams
******************************************************************************
* Summary:
*   This class contains connection parameters to connect to the AWS Endpoint
*****************************************************************************/
class AWSConnectionParams{

public:
	aws_publish_params publish_params;
	cy_rslt_t result;
	AWSIoTEndpoint* ep;
	AWSIoTClient client;
	char* message; 

	AWSConnectionParams(AWSIoTEndpoint* ep, AWSIoTClient client, aws_publish_params publish_params, char* message, cy_rslt_t result){

		this->ep = ep;
		this->client = client;
		this->publish_params = publish_params;
		this->result = result;
		this->message = message;
	}

};

/***************************************
* Function Prototypes
**************************************/
void RunCapSenseScan(void);
void InitTunerCommunication(void);
void ProcessTouchStatus(void);
void EZI2C_InterruptHandler(void);
void CapSense_InterruptHandler(void);
void CapSenseEndOfScanCallback(cy_stc_active_scan_sns_t * ptrActiveScan);
void InitCapSenseClock(void);
void AWSTask(AWSConnectionParams* awsArgs);

/*******************************************************************************
* Interrupt configuration
*******************************************************************************/
const cy_stc_sysint_t CapSense_ISR_cfg =
{
    .intrSrc = CYBSP_CSD_IRQ,
    .intrPriority = 4u
};

const cy_stc_sysint_t EZI2C_ISR_cfg = {
    .intrSrc = CYBSP_CSD_COMM_IRQ,
    .intrPriority = 3u
};

/*******************************************************************************
* Global variables
*******************************************************************************/
DigitalOut ledRed(LED_RED);
Semaphore capsense_sem;
EventQueue capsense_queue;
EventQueue aws_queue(32 * EVENTS_EVENT_SIZE);
Thread awsthread;
cy_stc_scb_ezi2c_context_t EZI2C_context;
uint32_t prevBtn0Status = 0u; 
uint32_t prevBtn1Status = 0u;
uint32_t prevSliderPos = 0u;
aws_connect_params conn_params = { 0,0,NULL,NULL,NULL,NULL,NULL };
aws_publish_params publish_params = { AWS_QOS_ATMOST_ONCE };
cy_rslt_t result = CY_RSLT_SUCCESS;
char buffer[50];
NetworkInterface* network;
AWSIoTClient client;
AWSIoTEndpoint* ep = NULL;

/* Initialize the class with default values */
AWSConnectionParams awsArgs(NULL, client, publish_params, AWSIOT_MESSAGE, CY_RSLT_SUCCESS);


/*****************************************************************************
* Function Name: initCapSense()
******************************************************************************
* Summary:
*   This function initialises the CapSense Block, sets up the interrupts
*   and the callbacks
*****************************************************************************/
void initCapSense(){

	/* Configure AMUX bus for CapSense */
    init_cycfg_routing();
    
    /* Configure PERI clocks for CapSense */
    InitCapSenseClock();    
    InitTunerCommunication();
    
    /* Initialize the CSD HW block to the default state. */
    cy_status status = Cy_CapSense_Init(&cy_capsense_context);
    if(CY_RET_SUCCESS != status)
    {
        printf("CapSense initialization failed. Status code: %lu\r\n", status);
        wait(osWaitForever);
    }
    
    /* Initialize CapSense interrupt */
    Cy_SysInt_Init(&CapSense_ISR_cfg, &CapSense_InterruptHandler);
    NVIC_ClearPendingIRQ(CapSense_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(CapSense_ISR_cfg.intrSrc);

    /* Initialize the CapSense firmware modules. */
    Cy_CapSense_Enable(&cy_capsense_context);
    Cy_CapSense_RegisterCallback(CY_CAPSENSE_END_OF_SCAN_E, CapSenseEndOfScanCallback, &cy_capsense_context);
   
}


/*****************************************************************************
* Function Name: AWSTask()
******************************************************************************
* Summary:
*   This function publishes the message to the AWS Cloud using the
*   IOT Publisher Client
*****************************************************************************/
void AWSTask(AWSConnectionParams* awsArgs){

	char* message;

	/* Get connection params from the class AWSConnectionParams */
	awsArgs->publish_params.QoS = AWS_QOS_ATMOST_ONCE;
	client = awsArgs->client;
	result = awsArgs->result;
	ep = awsArgs->ep;
	message = awsArgs->message;

	/* Publish the message */
	result = client.publish(ep, AWSIOT_TOPIC, message, strlen(message), publish_params);

	if (result != CY_RSLT_SUCCESS) {
		APP_INFO(("publish to topic failed \r\n"));
	}

	APP_INFO(("Published to topic successfully \r\n"));
	APP_INFO(("Published message: %s \r\n", message));

}

/*****************************************************************************
* Function Name: initAWSPublisher()
******************************************************************************
* Summary:
*   This function initialises the network interfaces, connects to the network
*   and establishes connection with the AWS endpoint
*****************************************************************************/
int initAWSPublisher(){

	APP_INFO(("Connecting to the network using Wifi...\r\n"));
    network = NetworkInterface::get_default_instance();

    nsapi_error_t net_status = -1;
    for (int tries = 0; tries < 3; tries++)
    {
        net_status = network->connect();
        if (net_status == NSAPI_ERROR_OK)
        {
            break;
        }
        else
        {
            APP_INFO(("Unable to connect to network. Retrying...\r\n"));
        }
    }

    if (net_status != NSAPI_ERROR_OK)
    {
        APP_INFO(("ERROR: Connecting to the network failed (%d)!\r\n", net_status));
        return -1;
    }

    APP_INFO(("Connected to the network successfully. IP address: %s\n", network->get_ip_address()));
	
	if ( ( strlen(SSL_CLIENTKEY_PEM) | strlen(SSL_CLIENTCERT_PEM) | strlen(SSL_CA_PEM) ) < 64 )
	{
		APP_INFO(("Please configure SSL_CLIENTKEY_PEM, SSL_CLIENTCERT_PEM and SSL_CA_PEM in aws_config.h file \n"));
		return -1;
	}

    /* Initialize AWS Client library */
    AWSIoTClient client(network, AWSIOT_THING_NAME , SSL_CLIENTKEY_PEM, strlen(SSL_CLIENTKEY_PEM), SSL_CLIENTCERT_PEM, strlen(SSL_CLIENTCERT_PEM));

    ep = client.create_endpoint(AWS_TRANSPORT_MQTT_NATIVE, AWSIOT_ENDPOINT_ADDRESS, AWS_IOT_SECURE_PORT, SSL_CA_PEM, strlen(SSL_CA_PEM));

    client.set_command_timeout( 5000 );

    /* set MQTT connection parameters */
    conn_params.username = NULL;
    conn_params.password = NULL;
    conn_params.keep_alive = AWSIOT_KEEPALIVE_TIMEOUT;
	conn_params.peer_cn = (uint8_t*) AWSIOT_ENDPOINT_ADDRESS;
    conn_params.client_id = (uint8_t*)AWSIOT_CLIENT_ID;

    /* connect to an AWS endpoint */
    result = client.connect (ep, conn_params);
    if ( result != CY_RSLT_SUCCESS )
    {
        APP_INFO(("connection to AWS endpoint failed \r\n"));
        return 1;
    }
    APP_INFO(("Connected to AWS endpoint \r\n"));

    /* Update the AWSConnectionParams class after initialization with the new values */
    awsArgs.ep = ep;
    awsArgs.client = client;
    awsArgs.publish_params = publish_params;
    awsArgs.result = result;
}


int main(void)
{

	/* Initialise publisher and CapSense Block */
	initAWSPublisher();
    initCapSense();

    /* Create thread to run CapSense scan periodically using an event queue
     * dispatcher and IOT Publisher Client on another thread
     */
    Thread capsense_thread(osPriorityNormal, OS_STACK_SIZE, NULL, "CapSense Scan Thread");
    Thread aws_dispatch_thread(osPriorityNormal, OS_STACK_SIZE, NULL, "AWS Publisher Thread");

    /* Run the event queue dispatcher on a thread for both CapSense and Publisher*/
    capsense_thread.start(callback(&capsense_queue, &EventQueue::dispatch_forever));
    aws_dispatch_thread.start(&aws_queue, &EventQueue::dispatch_forever);

    /* Schedule RunCapSenseScan() to run every scan period using the event queue */ 
    capsense_queue.call_every(CAPSENSE_SCAN_PERIOD, RunCapSenseScan);

    /* Call AWSTask using the event queue to publish messages */
    aws_queue.call(AWSTask, &awsArgs);

    /* Initiate scan immediately since the first call of RunCapSenseScan()
     * happens CAPSENSE_SCAN_PERIOD after the event queue dispatcher has
     * started. 
     */
    Cy_CapSense_ScanAllWidgets(&cy_capsense_context); 

    printf("Application has started. Touch any CapSense button or slider.\r\n");

    /* Put the main thread to sleep */
    wait(osWaitForever);
}


/*****************************************************************************
* Function Name: RunCapSenseScan()
******************************************************************************
* Summary:
*   This function starts the scan, and processes the touch status. It is
* periodically called by an event dispatcher. 
*
*****************************************************************************/
void RunCapSenseScan(void)
{
    Cy_CapSense_ScanAllWidgets(&cy_capsense_context);
    capsense_sem.acquire();          
    Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);
    Cy_CapSense_RunTuner(&cy_capsense_context);
    ProcessTouchStatus();     
}


/*******************************************************************************
* Function Name: InitTunerCommunication
********************************************************************************
*
* Summary:
*   This function performs the following functions:
*       - Initializes SCB block for operation in EZI2C mode.
*       - Connects EZI2C HW to the SDA and SCL pins.
*       - Sets communication data buffer to CapSense data structure.
*
*******************************************************************************/
void InitTunerCommunication(void)
{
    Cy_SCB_EZI2C_Init(CYBSP_CSD_COMM_HW, &CYBSP_CSD_COMM_config, &EZI2C_context);

    /* Initialize and enable EZI2C interrupts */
    Cy_SysInt_Init(&EZI2C_ISR_cfg, &EZI2C_InterruptHandler);
    NVIC_EnableIRQ(EZI2C_ISR_cfg.intrSrc);

    /* Set up communication data buffer to CapSense data structure to be exposed
     * to I2C master at primary slave address request.
     */
    Cy_SCB_EZI2C_SetBuffer1(CYBSP_CSD_COMM_HW, (uint8 *)&cy_capsense_tuner,
        sizeof(cy_capsense_tuner), sizeof(cy_capsense_tuner), &EZI2C_context);

    /* Enable EZI2C block */
    Cy_SCB_EZI2C_Enable(CYBSP_CSD_COMM_HW);
}


/*******************************************************************************
* Function Name: ProcessTouchStatus
********************************************************************************
*
* Summary:
*   Controls the LED status according to the status of CapSense widgets and
*   prints the status to serial terminal.
*
*******************************************************************************/
void ProcessTouchStatus(void)
{
    uint32_t currSliderPos;    
    uint32_t currBtn0Status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON0_WDGT_ID, CY_CAPSENSE_BUTTON0_SNS0_ID, &cy_capsense_context);        
    uint32_t currBtn1Status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON1_WDGT_ID, CY_CAPSENSE_BUTTON1_SNS0_ID, &cy_capsense_context);       
    cy_stc_capsense_touch_t *sldrTouch = Cy_CapSense_GetTouchInfo(CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context);

    if(currBtn0Status != prevBtn0Status)
    {
        sprintf(buffer, "Button_0 status: %lu\r\n", currBtn0Status);
        printf("Button_0 status: %lu\r\n", currBtn0Status);
        prevBtn0Status = currBtn0Status;	
        awsArgs.message = buffer;
        aws_queue.call(AWSTask, &awsArgs);
    }
    
    if(currBtn1Status != prevBtn1Status)
    {
        sprintf(buffer, "Button_1 status: %lu\r\n", currBtn1Status);
        printf("Button_1 status: %lu\r\n", currBtn1Status);
        prevBtn1Status = currBtn1Status;
        awsArgs.message = buffer;
        aws_queue.call(AWSTask, &awsArgs);
    } 

    if (sldrTouch->numPosition == SLIDER_NUM_TOUCH)
    {       
        currSliderPos = sldrTouch->ptrPosition->x;

        if(currSliderPos != prevSliderPos)
        {
            sprintf(buffer, "Slider position: %lu\r\n", currSliderPos);
            printf("Slider position: %lu\r\n", currSliderPos);
            prevSliderPos = currSliderPos;
            awsArgs.message = buffer;
            aws_queue.call(AWSTask, &awsArgs);
        }
    }

    ledRed = (currBtn0Status || currBtn1Status || (sldrTouch->numPosition == SLIDER_NUM_TOUCH)) ? LED_ON : LED_OFF;
}


/*******************************************************************************
* Function Name: EZI2C_InterruptHandler
********************************************************************************
* Summary:
*   Wrapper function for handling interrupts from EZI2C block. 
*
*******************************************************************************/
void EZI2C_InterruptHandler(void)
{
    Cy_SCB_EZI2C_Interrupt(CYBSP_CSD_COMM_HW, &EZI2C_context);
}

/*****************************************************************************
* Function Name: CapSense_InterruptHandler()
******************************************************************************
* Summary:
*  Wrapper function for handling interrupts from CSD block.
*
*****************************************************************************/
void CapSense_InterruptHandler(void)
{
    Cy_CapSense_InterruptHandler(CYBSP_CSD_HW, &cy_capsense_context);
}


/*****************************************************************************
* Function Name: CapSenseEndOfScanCallback()
******************************************************************************
* Summary:
*  This function releases a semaphore to indicate end of a CapSense scan.
*
* Parameters:
*  cy_stc_active_scan_sns_t* : pointer to active sensor details.
*
*****************************************************************************/
void CapSenseEndOfScanCallback(cy_stc_active_scan_sns_t * ptrActiveScan)
{
    capsense_sem.release();
}


/*****************************************************************************
* Function Name: InitCapSenseClock()
******************************************************************************
* Summary:
*  This function configures the peripheral clock for CapSense.  
*
*****************************************************************************/
void InitCapSenseClock(void)
{
    Cy_SysClk_PeriphAssignDivider(PCLK_CSD_CLOCK, CYBSP_CSD_CLK_DIV_HW, CYBSP_CSD_CLK_DIV_NUM);
    Cy_SysClk_PeriphDisableDivider(CYBSP_CSD_CLK_DIV_HW, CYBSP_CSD_CLK_DIV_NUM);
    Cy_SysClk_PeriphSetDivider(CYBSP_CSD_CLK_DIV_HW, CYBSP_CSD_CLK_DIV_NUM, 0u);
    Cy_SysClk_PeriphEnableDivider(CYBSP_CSD_CLK_DIV_HW, CYBSP_CSD_CLK_DIV_NUM);
}
