# Plant Pot

This is an IoT project developed using an ESP32 module. It leverages the ESP32's Wi-Fi capabilities to serve a web page that displays the moisture level of a plant's soil in real-time. 

<img width="523" alt="Screenshot 2024-03-15 at 17 44 09" src="https://github.com/Zdrenka/plant_pot/assets/2591040/6d33f653-16e7-478a-98d5-0dab8af44c53">

The project uses Server-Sent Events (SSE) to push updates from the ESP32 to a web client without the need for the client to refresh the page.

## Features

- Real-time soil moisture level monitoring via a web interface.
- Non-blocking web server implementation on ESP32 using `ESPAsyncWebServer`.
- Real-time updates to the web page using Server-Sent Events (SSE).

## Hardware Requirements

- ESP32 Development Board
- Soil Moisture Sensor (connected to AOUT_PIN)
- OLED Display (128x64, connected via SPI)
- Optional: LEDs or other indicators for direct feedback from the device

## Software Dependencies

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)

## Setup and Installation

1. **Connect the Hardware**: Follow the wiring instructions for your specific ESP32 module and peripherals.
2. **Configure Wi-Fi**: Update the `ssid` and `password` variables in the code to match your Wi-Fi network credentials.
3. **Install Dependencies**: Make sure to install all required libraries through the Arduino IDE or your preferred development environment.
4. **Upload the Sketch**: Compile and upload the sketch to your ESP32.

## Usage

After uploading the sketch and connecting the ESP32 to your Wi-Fi network, the device will start monitoring the soil moisture level. To view the moisture level:

1. Open a web browser on a device connected to the same Wi-Fi network.
2. Enter the IP address of the ESP32 (displayed in the Serial Monitor after connecting to Wi-Fi).
3. The web page will display the current moisture level, which updates in real-time.

## Customizing the Web Interface

The HTML/CSS for the web interface is embedded within the sketch. You can modify the `htmlContent` string to customize the appearance of the web page. Additional plant-themed elements or styles can be added to enhance the user experience.
