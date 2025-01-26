# ESP32 E-Paper Weather Display

This is a weather display powered by a wifi-enabled ESP32 microcontroller and a 7.5in E-Paper (aka E-ink) display. Current and forecasted weather data is obtained from the OpenWeatherMap API. A sensor provides the display with accurate indoor temperature and humidity.

<p float="left">
  <img src="showcase/assembled-demo-raleigh-front.jpg" />
  <img src="showcase/assembled-demo-raleigh-side.jpg" width="49%" />
  <img src="showcase/assembled-demo-raleigh-back.jpg" width="49%" />
  <img src="showcase/assembled-demo-bottom-cover.jpg" width="49%" />
  <img src="showcase/assembled-demo-bottom-cover-removed.jpg" width="49%" />
</p>

The project draws ~14μA when sleeping and an estimated average of ~83mA during its ~15s wake period. The display can be configured to update as frequently as desired. When the refresh interval is set to 30 minutes, the device will run for >6 months on a single 5000mAh battery. The project displays accurate battery life percentage and can be recharged via a USB-C cable connected to a wall adapter or computer.

There are configuration options for everything from location, time/date formats, units, and language to air quality index scale and hourly outlook graph bounds.

The hourly outlook graph (bottom right) shows a line indicating temperature and shaded bars indicating probability of precipitation (or optionally volume of precipitation).

Here are two examples utilizing various configuration options:

<p float="left">
  <img src="showcase/demo-new-york.jpg" width="49%" />
  <img src="showcase/demo-london.jpg" width="49%" />
</p>


## Contents

-   [Setup Guide](#setup-guide)
    -   [Hardware](#hardware)
    -   [Wiring](#wiring)
    -   [Configuration, Compilation, and Upload](#configuration-compilation-and-upload)
    -   [OpenWeatherMap API Key](#openweathermap-api-key)
-   [Error Messages and Troubleshooting](#error-messages-and-troubleshooting)
    -   [Low Battery](#low-battery)
    -   [WiFi Connection](#wifi-connection)
    -   [API Error](#api-error)
    -   [Time Server Error](#time-server-error)
-   [Licensing](#licensing)


## Setup Guide

### Hardware

7.5inch (800×480) E-Paper Display

- Advantages of E-Paper
  - Ultra Low Power Consumption - E-Paper (aka E-Ink) displays are ideal for low-power applications that do not require frequent display refreshes. E-Paper displays only draw power when refreshing the display and do not have a backlight. Images will remain on the screen even when power is removed.

- Limitations of E-Paper:
  - Colors - E-Paper has traditionally been limited to just black and white, but in recent years 3-color E-Paper screens have started showing up.

  - Refresh Times and Ghosting - E-Paper displays are highly susceptible to ghosting effects if refreshed too quickly. To avoid this, E-Paper displays often take a few seconds to refresh(4s for the unit used in this project) and will alternate between black and white a few times, which can be distracting.

- Panel support:

  Waveshare and Good Display make equivalent panels. Either variant will work.

  | Panel                                   | Resolution | Colors          | Notes                                                                                                                 |
  |-----------------------------------------|------------|-----------------|-----------------------------------------------------------------------------------------------------------------------|
  | Waveshare 7.5in e-paper (v2)            | 800x480px  | Black/White     | Available [here](https://www.waveshare.com/product/7.5inch-e-paper.htm). (recommended)                                |
  | Good Display 7.5in e-paper (GDEY075T7)  | 800x480px  | Black/White     | Available [here](https://www.aliexpress.com/item/3256802683908868.html). (recommended)                                |
  | Waveshare 7.5in e-Paper (B)             | 800x480px  | Red/Black/White | Available [here](https://www.waveshare.com/product/7.5inch-e-paper-b.htm).                                            |
  | Good Display 7.5in e-paper (GDEY075Z08) | 800x480px  | Red/Black/White | Available [here](https://www.aliexpress.com/item/3256803540460035.html).                                              |
  | Waveshare 7.3in ACeP e-Paper (F)        | 800x480px  | 7-Color         | Available [here](https://www.waveshare.com/product/displays/e-paper/epaper-1/7.3inch-e-paper-f.htm).                  |
  | Good Display 7.3in e-paper (GDEY073D46) | 800x480px  | 7-Color         | Available [here](https://www.aliexpress.com/item/3256805485098421.html).                                              |
  | Waveshare 7.5in e-paper (v1)            | 640x384px  | Black/White     | Limited support. Some information not displayed, see [image](showcase/demo-waveshare75-version1.jpg).                 |
  | Good Display 7.5in e-paper (GDEW075T8)  | 640x384px  | Black/White     | Limited support. Some information not displayed, see [image](showcase/demo-waveshare75-version1.jpg).                 |

  This software has limited support for accent colors. E-paper panels with additional colors tend to have longer refresh times, which will reduce battery life.

DESPI-C02 Adapter Board

- No level converters, which makes it better for low-power use with 3.3V processors compared to the Waveshare HAT.

- The Waveshare HATs (rev 2.2/2.3) are not recommended. Their compatibility with this project is not regularly tested.

- https://www.e-paper-display.com/products_detail/productId=403.html
  
- https://www.aliexpress.us/item/3256804446769469.html


FireBeetle 2 ESP32-E Microcontroller

- Why the ESP32?

  - Onboard WiFi.

  - 520kB of RAM and 4MB of FLASH, enough to store lots of icons and fonts.

  - Low power consumption.

  - Small size, many small development boards available.

- Why the FireBeetle 2 ESP32-E

  - Drobot's FireBeetle ESP32 models are optimized for low-power consumption (<https://diyi0t.com/reduce-the-esp32-power-consumption/>). The Drobot's FireBeetle 2 ESP32-E variant offers USB-C, but older versions of the board with Micro-USB would work fine too.

  - Firebeetle ESP32 models include onboard charging circuitry for a 3.7v lithium-ion(LiPo) battery.

  - FireBeetle ESP32 models include onboard circuitry to monitor battery voltage of a battery connected to its JST-PH2.0 connector.


- <https://www.dfrobot.com/product-2195.html>


BME280 - Pressure, Temperature, and Humidity Sensor


- Provides accurate indoor temperature and humidity.

- Much faster than the DHT22, which requires a 2-second wait before reading temperature and humidity samples.


3.7V Lipo Battery w/ 2 Pin JST Connector


- Size is up to you. I used a 5000mah battery so that the device can operate on a single charge for >6 months.


- The battery can be charged by plugging the FireBeetle ESP32 into the wall via the USB-C connector while the battery is plugged into the ESP32's JST connector.

  > **Warning**
  > The polarity of JST-PH2.0 connectors is not standardized! You may need to swap the order of the wires in the connector.

Stand/Frame
- You'll want a nice way to show off your project. Here are a few popular choices.
- DIY Wooden
  - I made a small stand by hollowing out a piece of wood from the bottom. On the back, I used a short USB extension cable so that I can charge the battery without needing to remove the components from the stand. I also wired a small reset button to refresh the display manually. Additionally, I 3d printed a cover for the bottom, which is held on by magnets. The E-paper screen is very thin, so I used a thin piece of acrylic to support it.
  - Measurements:
    - depth = 63mm <br>
      height = 49mm <br>
      width = 170.2mm (= width of the screen) <br>
      screen angle = 80deg <br>
      screen is 15mm from the front
- 3D Printable
  - Here is a list of community designs.
  
    | Contributor                                                          | Link                                                                                                     |
    |----------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------|
    | [Kingfisher](https://www.printables.com/@Kingfisher_32821)           | [Printables](https://www.printables.com/model/1139047-weather-station-e-ink-frame)                       |
    | [Francois Allard](https://www.printables.com/@FrAllard_1585397)      | [Printables](https://www.printables.com/model/791477-weather-station-using-a-esp32)                      |
    | [3D Nate](https://www.printables.com/@3DNate_451157)                 | [Printables](https://www.printables.com/model/661183-e-ink-weather-station-frame)                        |
    | [Sven F.](https://github.com/Spanholz)                               | [Printables](https://www.printables.com/model/657756-case-for-esp32-weather-station)                     |
    | [Layers Studio](https://www.printables.com/@LayersStudio)            | [Printables](https://www.printables.com/model/655768-esp32-e-paper-weather-display-stand)                |
    | [PJ Veltri](https://www.printables.com/@PJVeltri_1590999)            | [Printables](https://www.printables.com/model/692944-base-and-display-holder-for-esp-32-e-paper-weather) |
    | [TheMeanCanEHdian](https://www.printables.com/@TheMeanCanEH_1207348) | [Printables](https://www.printables.com/model/841458-weather-display-enclosure)                          |
    | [MPHarms](https://www.thingiverse.com/mpharms/designs)               | [Thingiverse](https://www.thingiverse.com/thing:6666148)                                                 |
    | [Plaste-Metz](https://www.printables.com/@PlasteMetz_576567)         | [Printables](https://www.printables.com/model/1160924-weather-station-case)                              |

  - If you want to share your own 3D printable designs, your contributions are highly encouraged and welcome!
- Picture Frame


### Wiring

Pin connections are defined in [config.cpp](platformio/src/config.cpp).

If you are using the FireBeetle 2 ESP32-E, you can use the connections I used or change them how you would like.

I have included 2 wiring diagrams. One for the Waveshare HAT rev2.2 and another using the recommended DESPI-C02.

NOTE: Waveshare now ships revision 2.3 of their e-paper HAT (no longer rev 2.2 ). Rev 2.3 has an additional `PWR` pin (not depicted in the wiring diagrams below); connect this pin to 3.3V.

IMPORTANT: The Waveshare E-Paper Driver HAT has two physical switches that MUST be set correctly for the display to work.

- Display Config: Set switch to position B.

- Interface Config: Set switch to position 0.

IMPORTANT: The DESPI-C02 adapter has one physical switch that MUST be set correctly for the display to work.

- RESE: Set switch to position 0.47.

Cut the low power pad for even longer battery life.

- From <https://wiki.dfrobot.com/FireBeetle_Board_ESP32_E_SKU_DFR0654>

  > Low Power Pad: This pad is specially designed for low power consumption. It is connected by default. You can cut off the thin wire in the middle with a knife to disconnect it. After disconnection, the static power consumption can be reduced by 500 μA. The power consumption can be reduced to 13 μA after controlling the maincontroller enter the sleep mode through the program. Note: when the pad is disconnected, you can only drive RGB LED light via the USB Power supply.

<p float="left">
  <img src="showcase/wiring_diagram_despi-c02.png" width="49%" />
  <img src="showcase/wiring_diagram_waveshare_rev22.png" width="49%" />
  <img src="showcase/demo-tucson.jpg" width="32%" />
</p>


### Configuration, Compilation, and Upload

PlatformIO for VSCode is used for managing dependencies, code compilation, and uploading to ESP32.

1. Clone this repository or download and extract the .zip.

2. Install VSCode.

3. Follow these instructions to install the PlatformIO extension for VSCode: <https://platformio.org/install/ide?install=vscode>

4. Open the project in VSCode.

   a. File > Open Folder...

   b. Navigate to this project and select the folder called "platformio".

5. Configure Options.

   - Most configuration options are located in [config.cpp](platformio/src/config.cpp), with a few  in [config.h](platformio/include/config.h). Locale/language options can also be found in include/locales/locale_*.inc.

   - Important settings to configure in config.cpp:

     - WiFi credentials (ssid, password).

     - Open Weather Map API key (it's free, see next section for important notes about obtaining an API key).

     - Latitude and longitude.

     - Time and date formats.

     - Sleep duration.

     - Pin connections for E-Paper (SPI), BME280 (I2C), and battery voltage (ADC).

   - Important settings to configure in config.h:

     - Units (Metric or Imperial).

   - Comments explain each option in detail.

6. Build and Upload Code.

   a. Connect ESP32 to your computer via USB.

   b. Click the upload arrow along the bottom of the VSCode window. (Should say "PlatformIO: Upload" if you hover over it.)

      - PlatformIO will automatically download the required third-party libraries, compile, and upload the code. :)

      - You will only see this if you have the PlatformIO extension installed.

      - If using a FireBeetle 2 ESP32-E and you receive the error `Wrong boot mode detected (0x13)! The chip needs to be in download mode.` unplug the power from the board, connect GPIO0 ([labeled 0/D5](https://wiki.dfrobot.com/FireBeetle_Board_ESP32_E_SKU_DFR0654#target_5)) to GND, and power it back up to put the board in download mode.

      - If you are getting other errors during the upload process, you may need to install drivers to allow you to upload code to the ESP32.
### OpenWeatherMap API Key

Sign up here to get an API key; it's free. <https://openweathermap.org/api>

This project will make calls to 2 different APIs ("One Call" and "Air Pollution").

- The One Call API 3.0 is only included in the "One Call by Call" subscription. This separate subscription includes 1,000 calls/day for free and allows you to pay only for the number of API calls made to this product.

Here's how to subscribe and avoid any credit card changes:
   - Go to <https://home.openweathermap.org/subscriptions/billing_info/onecall_30/base?key=base&service=onecall_30>
   - Follow the instructions to complete the subscription.
   - Go to <https://home.openweathermap.org/subscriptions> and set the "Calls per day (no more than)" to 1,000. This ensures you will never overrun the free calls.

## Error Messages and Troubleshooting

### Low Battery
<img src="showcase/demo-error-low-battery.jpg" align="left" width="25%" />
This error screen appears once the battery voltage has fallen below LOW_BATTERY_VOLTAGE (default = 3.20v). The display will not refresh again until it detects battery voltage above LOW_BATTERY_VOLTAGE. When battery voltage is between LOW_BATTERY_VOLTAGE and VERY_LOW_BATTERY_VOLTAGE (default = 3.10v) the esp32 will deep-sleep for periods of LOW_BATTERY_SLEEP_INTERVAL (default = 30min) before checking battery voltage again. If the battery voltage falls between LOW_BATTERY_SLEEP_INTERVAL and CRIT_LOW_BATTERY_VOLTAGE (default = 3.00v), then the display will deep-sleep for periods VERY_LOW_BATTERY_SLEEP_INTERVAL (default = 120min). If battery voltage falls below CRIT_LOW_BATTERY_VOLTAGE, then the esp32 will enter hibernate mode and will require a manual push of the reset (RST) button to begin updating again.

<br clear="left"/>

### WiFi Connection
<img src="showcase/demo-error-wifi.jpg" align="left" width="25%" />
This error screen appears when the ESP32 fails to connect to WiFi. If the message reads "WiFi Connection Failed" this might indicate an incorrect password. If the message reads "SSID Not Available" this might indicate that you mistyped the SSID or that the esp32 is out of the range of the access point. The esp32 will retry once every SLEEP_DURATION (default = 30min).

<br clear="left"/>

### API Error
<img src="showcase/demo-error-api.jpg" align="left" width="25%" />
This error screen appears if an error (client or server) occurs when making an API request to OpenWeatherMap. The second line will give the error code followed by a descriptor phrase. Positive error codes correspond to HTTP response status codes, while error codes <= 0 indicate a client(esp32) error. The esp32 will retry once every SLEEP_DURATION (default = 30min).
<br/><br/>
In the example shown to the left, "401: Unauthorized" may be the result of an incorrect API key or that you are attempting to use the One Call v3 API without the proper account setup.

<br clear="left"/>

### Time Server Error
<img src="showcase/demo-error-time.jpg" align="left" width="25%" />
This error screen appears when the esp32 fails to fetch the time from NTP_SERVER_1/NTP_SERVER_2. This error sometimes occurs immediately after uploading to the esp32; in this case, just hit the reset button or wait for SLEEP_DURATION (default = 30min) and the esp32 to automatically retry. If the error persists, try selecting closer/lower latency time servers or increasing NTP_TIMEOUT.

<br clear="left"/>

## Licensing

esp32-weather-epd is licensed under the [GNU General Public License v3.0](LICENSE) with tools, fonts, and icons whose licenses are as follows:

| Name | License | Description |
|---------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------|------------------------------------------------------------------------------------|
| [Adafruit-GFX-Library: fontconvert](https://github.com/adafruit/Adafruit-GFX-Library/tree/master/fontconvert) | [BSD License](fonts/fontconvert/license.txt) | CLI tool for preprocessing fonts to be used with the Adafruit_GFX Arduino library. |
| [pollutant-concentration-to-aqi](https://github.com/lmarzen/pollutant-concentration-to-aqi) | [GNU Lesser General Public License v2.1](platformio/lib/pollutant-concentration-to-aqi/LICENSE) | C library that converts pollutant concentrations to Air Quality Index(AQI). |
| [GNU FreeFont](https://www.gnu.org/software/freefont/) | [GNU General Public License v3.0](https://www.gnu.org/software/freefont/license.html) | Font Family |
| [Lato](https://fonts.google.com/specimen/Lato) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | Font Family |
| [Montserrat](https://fonts.google.com/specimen/Montserrat) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | Font Family |
| [Open Sans](https://fonts.google.com/specimen/Open+Sans) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | Font Family |
| [Poppins](https://fonts.google.com/specimen/Poppins) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | Font Family |
| [Quicksand](https://fonts.google.com/specimen/Quicksand) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | Font Family |
| [Raleway](https://fonts.google.com/specimen/Raleway) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | Font Family |
| [Roboto](https://fonts.google.com/specimen/Roboto) | [Apache License v2.0](https://www.apache.org/licenses/LICENSE-2.0) | Font Family |
| [Roboto Mono](https://fonts.google.com/specimen/Roboto+Mono) | [Apache License v2.0](https://www.apache.org/licenses/LICENSE-2.0) | Font Family |
| [Roboto Slab](https://fonts.google.com/specimen/Roboto+Slab) | [Apache License v2.0](https://www.apache.org/licenses/LICENSE-2.0) | Font Family |
| [Ubuntu font](https://design.ubuntu.com/font) | [Ubuntu Font Licence v1.0](https://ubuntu.com/legal/font-licence) | Font Family |
| [Weather Themed Icons](https://github.com/erikflowers/weather-icons) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | (wi-**.svg) Weather icon family by Lukas Bischoff/Erik Flowers. |
| [Google Icons](https://fonts.google.com/icons) | [Apache License v2.0](https://www.apache.org/licenses/LICENSE-2.0) | (battery**.svg, visibility_icon.svg) Battery and visibility icons from Google Icons. |
| [Biological Hazard Symbol](https://svgsilh.com/image/37775.html) | [CC0 v1.0](https://en.wikipedia.org/wiki/Public_domain) | (biological_hazard_symbol.svg) Biohazard icon. |
| [House Icon](https://seekicon.com/free-icon/house_16) | [MIT License](http://opensource.org/licenses/mit-license.html) | (house.svg) House icon. |
| [Indoor Temerature/Humidity Icons](icons/svg) | [SIL OFL v1.1](http://scripts.sil.org/OFL) | (house_**.svg) Indoor temerature/humidity icons. |
| [Ionizing Radiation Symbol](https://svgsilh.com/image/309911.html) | [CC0 v1.0](https://creativecommons.org/publicdomain/zero/1.0/) | (ionizing_radiation_symbol.svg) Ionizing radiation icons. |
| [Phosphor Icons](https://github.com/phosphor-icons/homepage) | [MIT License](http://opensource.org/licenses/mit-license.html) | (wifi**.svg, warning_icon.svg, error_icon.svg) WiFi, Warning, and Error icons from Phosphor Icons. |
| [Wind Direction Icon](https://www.onlinewebfonts.com/icon/251550) | [CC BY v3.0](http://creativecommons.org/licenses/by/3.0) | (meteorological_wind_direction_**deg.svg) Meteorological wind direction icon from Online Web Fonts. |

