# MbedOS_Examples
Cypress Semiconductor Mbed OS page has several examples but each deal with a single component block. This project demonstrates how to combine the IOT Publisher and CapSense code examples. When the CapSense slider/buttons are pressed, the values get published on to the AWS Cloud using an MQTT channel.

# Instructions to run the AWSPublisher_CapSenseSlider code example

1. Import the code example

        mbed import https://github.com/Dheeraj22/MbedOS_Examples

2. Change working directory to the example folder  
   
        cd AWSCapSenseSlider

3. prepare the cloned working directory for mbed
        
        mbed config root .

4. pull the necessary libraries and its dependencies.
This will pull mbed-os, AWS_Iot_Client library and its internal 3rd party dependencies

        mbed deploy

5. Configure the AWS parameters such as Thing name, certificates, private key etc. per the user's AWS account
	- Refer to 'Getting Started with AWS IoT' on the AWS documentation
	    - https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html
	- Using the above reference, fill out the aws_config.h parameters

6. Plug in the CY8CPROTO-062-4343W kit. Ensure that the kit has the latest version of KitProg3 firmware and is in DAPLink mode to allow programming from Mbed CLI. See [Firmware-loader](https://github.com/cypresssemiconductorco/Firmware-loader) for more information on how to update the firmware and switch to DAPLink mode. 

7. Ensure the kit is configured to operate at 3.3 V. See the kit user guide to learn how to configure the power supply. 

    **Note**: The project works only with VDD = 3.3 V. 

8. Compile the example and Program

        mbed compile --target CY8CPROTO_062_4343W --toolchain GCC_ARM --flash --sterm
        
        For other targets:
        mbed compile -m CY8CKIT_062_WIFI_BT -t GCC_ARM -f --sterm

    **Note:** The *--sterm* option opens the serial terminal with 9600-8N1 setting on the command prompt itself after programming completes. Do not use this option if you wish to connect using another serial terminal application.

9. Following message is displayed on the serial terminal when the application starts running.

        Application has started. Touch any CapSense button or slider.

10. Touch the buttons or the slider to observe the red LED changing its state and the status printed on the serial terminal. 
