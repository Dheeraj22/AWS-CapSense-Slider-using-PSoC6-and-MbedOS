# AWSPublisher_CapSenseSlider code example
[Cypress Semiconductor MbedOS](https://os.mbed.com/teams/Cypress/) page has several examples but each deal with a single component block. This project demonstrates how to combine the IOT Publisher and CapSense code examples. When the CapSense slider/buttons are pressed, the values get published on to the AWS Cloud using an MQTT channel.

# Instructions to run the code example

1. Import the code example

        mbed import https://github.com/Dheeraj22/MbedOS_Examples

2. Change working directory to the example folder  
   
        cd "MbedOS_Examples\AWSCapSenseSlider"

3. prepare the cloned working directory for mbed
        
        mbed config root .

4. pull the necessary libraries and its dependencies.
This will pull mbed-os, AWS_Iot_Client library and its internal 3rd party dependencies

        mbed deploy

5. Configure the AWS parameters such as Thing name, certificates, private key etc. per the user's AWS account
	- Refer to 'Getting Started with AWS IoT' on the AWS documentation
	    - https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html
	- Using the above reference, fill out the aws_config.h parameters
	- Add SSID and Password in mbed_app.json file

6. Plug in the CY8CPROTO-062-4343W kit. Ensure that the kit has the latest version of KitProg3 firmware and is in DAPLink mode to allow programming from Mbed CLI. See [Firmware-loader](https://github.com/cypresssemiconductorco/Firmware-loader) for more information on how to update the firmware and switch to DAPLink mode. 

7. Ensure the kit is configured to operate at 3.3 V. See the kit user guide to learn how to configure the power supply. 

    **Note**: The project works only with VDD = 3.3 V. 

8. Compile the example and Program

        mbed compile --target CY8CPROTO_062_4343W --toolchain GCC_ARM --flash

9. Connect a serial terminal at 115200 baud rate

	```
	mbed sterm -b 115200
	```
	
        For other targets:
        mbed compile -m CY8CKIT_062_WIFI_BT -t GCC_ARM -f --sterm

    **Note:** The *--sterm* option opens the serial terminal with 9600-8N1 setting on the command prompt itself after programming completes. Do not use this option if you wish to connect using another serial terminal application.

9. Following message is displayed on the serial terminal when the application starts running.

        Application has started. Touch any CapSense button or slider.

10. Touch the buttons or the slider to observe the red LED changing its state and the status printed on the serial terminal. On the AWS IOT Dashboard, setup an MQTT Client to subscribe to a topic to receive the published messages. 

# Output Log
``` 
C:\MbedWorkspace\MbedOS_Examples\AWSCapSenseSlider>mbed sterm -b 115200
[mbed] Working path "C:\MbedWorkspace\MbedOS_Examples\AWSCapSenseSlider" (library)
[mbed] Program path "C:\MbedWorkspace\MbedOS_Examples\AWSCapSenseSlider"
[mbed] Detecting connected targets/boards to your system...
[mbed] Opening serial terminal to "CY8CPROTO_062_4343W"
--- Terminal on COM85 - 115200,8,N,1 ---
Connecting to the network using Wifi...
WLAN MAC Address : 00:9D:6B:64:5F:F1
WLAN Firmware    : wl0: Jun  7 2019 07:11:39 version 7.45.98.87 (r717217 CY) FWID 01-4cc34fdc
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2019-06-07 06:59:52
WHD VERSION      : 1b9fceb7 : master : GCC 6.3 : 2019-07-03 16:07:52 +0530
Connected to the network successfully. IP address: 192.168.43.247
Connected to AWS endpoint
Application has started. Touch any CapSense button or slider.
Published to topic successfully
Published message: Starting AWS MQTT Connection
Button_0 status: 1
Published to topic successfully
Published message: Button_0 status: 1

Button_0 status: 0
Published to topic successfully
Published message: Button_0 status: 0

Button_1 status: 1
Published to topic successfully
Published message: Button_1 status: 1

Button_1 status: 0
Published to topic successfully
Published message: Button_1 status: 0

Slider position: 2
Published to topic successfully
Published message: Slider position: 2

Slider position: 3
Published to topic successfully
Published message: Slider position: 3

Slider position: 5
Published to topic successfully
Published message: Slider position: 5

Slider position: 7
Published to topic successfully
Published message: Slider position: 7

Slider position: 9
Published to topic successfully
Published message: Slider position: 9

Slider position: 11
Published to topic successfully
Published message: Slider position: 11

Slider position: 16
Published to topic successfully
Published message: Slider position: 16

Slider position: 19
Published to topic successfully
Published message: Slider position: 19

Slider position: 24
Published to topic successfully
Published message: Slider position: 24

Slider position: 27
Published to topic successfully
Published message: Slider position: 27

Slider position: 30
Published to topic successfully
Published message: Slider position: 30

Slider position: 33
Published to topic successfully
Published message: Slider position: 33

Slider position: 41
Published to topic successfully
Published message: Slider position: 41

Slider position: 46
Published to topic successfully
Published message: Slider position: 46

Slider position: 52
Published to topic successfully
Published message: Slider position: 52

Slider position: 55
Published to topic successfully
Published message: Slider position: 55

Slider position: 58
Published to topic successfully
Published message: Slider position: 58

Slider position: 65
Published to topic successfully
Published message: Slider position: 65

Slider position: 68
Published to topic successfully
Published message: Slider position: 68

Slider position: 72
Published to topic successfully
Published message: Slider position: 72

Slider position: 76
Published to topic successfully
Published message: Slider position: 76

Slider position: 78
Published to topic successfully
Published message: Slider position: 78

Slider position: 81
Published to topic successfully
Published message: Slider position: 81

Slider position: 83
Published to topic successfully
Published message: Slider position: 83

Slider position: 87
Published to topic successfully
Published message: Slider position: 87

Slider position: 99
Published to topic successfully
Published message: Slider position: 99

Published to topic successfully
Published message: Slider position: 99

Published to topic successfully
Published message: Slider position: 99

Published to topic successfully
Published message: Slider position: 99

Published to topic successfully
Published message: Slider position: 99
