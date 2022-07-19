# ESP32 E-Paper Weather Display

.

.

.


## Setup Guide

### Components

7.5inch (800×480) E-Ink Display w/ HAT for Raspberry Pi, SPI interface

- Advantages of E-Paper
  - Ultra Low Power Consumption - E-Paper (or E-Ink) displays are ideal for low-power applications that do not require frequent display refreshes. E-Paper displays only draw power when refreshing the display and do not have a backlight. Images will remain on the screen even when power is removed.

- Limitations of E-Paper: 
  - Colors - E-Paper has traditionally been limited to just black and white, but in recent years 3-color E-Paper screens are started to show up.

  - Refresh Times and Ghosting - E-Paper displays are highly susceptible to ghosting effects if refreshed to quickly. To avoid this to  E-Paper displays often take a few seconds to refresh(4s for the unit used in this project) and will alternate between black and white a few times which can be distracting.  


- https://www.waveshare.com/product/7.5inch-e-paper-hat.htm


FireBeetle 2 ESP32-E Microcontroller

- Why the ESP32?

  - Onboard WiFi.

  - 520kB of RAM and 4MB of FLASH. Enough to store lots icons and fonts.

  - Low power consumption.

  - Small size, many small development boards available.

- Why the FireBeetle 2 ESP32-E?

  - Drobot's FireBeetle ESP32 models is optimized for low-power consumption (https://diyi0t.com/reduce-the-esp32-power-consumption/). The Drobot's FireBeetle 2 ESP32-E variant offers USB-C, but older versions of the board with Mirco-USB would work just fine too.

  - Firebeelte ESP32 models include onboard charging circuitry for lithium ion batteries.

  - FireBeetle ESP32 models include onboard circuitry to monitor battery voltage of a battery connected to its JST-PH2.0 connector.


- https://www.dfrobot.com/product-2195.html


BME280 - Pressure, Temperature, and Humidity Sensor


- Provides accurate indoor temperature and humidity.

- Much faster than the DHT22 which requires a 2 second wait before reading temperature and humidity samples.


3.7V Lipo Battery w/ 2 Pin JST Connector 


- Size is up to you. I used a 10000mah battery so that the device can operate on a single charge for 1.5-2 years.


- The battery can be charged by plugging the FireBeetle ESP32 into the wall via the USB-C connector while the battery is plugged into the ESP32's JST connector.


- WARNING: The polarity of JST-PH2.0 connectors is not standardized! You may need to swap order of the wires in the connector.


### Wiring

Pin connections can be defined in config.cpp. 

If you are using the FireBeetle 2 ESP32-E you can use the connections I used or change them how you would like.

The E-Paper Driver Hat has two physical switches that must be set correctly in order for the display to work.

- Display Config: Set switch to position B.

- Interface Config: Set switch to position 1.


### Configuration, Compilation, and Upload

PlatformIO for VSCode is used for managing dependencies, code compilation, and upload to ESP32.

1. Clone this repository or download and extract the .zip.

2. Install VSCode.

3. Follow these instructions to install the PlatformIO extension for VSCode: https://platformio.org/install/ide?install=vscode

4. Open the project in VSCode.

   a. File > Open Folder...

   b. Navigate to this project and select the folder called "platformio".

5. Configure Options.

   - Most configuration options are located in config.cpp with the a few in config.h. Language options can also be found in lang_en_us.cpp.

   - Important settings to configure in config.cpp:

     - WiFi credentials (ssid, password).

     - Open Weather Map API key (sign up here to get an API key, its free. https://openweathermap.org/api).

     - Latitude and longitude.

     - Time and date formats.

     - Sleep duration.

     - Pin connections for E-Paper (SPI), BME280 (I2C), and battery voltage (ADC).

   - Important settings to configure in config.h:

     - Units (Metric or Imperial).

   - Comments explain each option in detail.

6. Build and Upload Code.

   a. Connect ESP32 to Computer via USB.

   b. Click the upload arrow along the bottom of the VSCode window. (Should say "PlatformIO: Upload" if you hover over it.) 

      - PlatformIO will automatically download the required third-party libraries, then will compile and upload the code. :)
     
      - You will not see this if you don't have the PlatformIO extension installed.

      - If you are getting errors during the upload process, you may need to install drivers to allow you to upload code to the ESP32.
