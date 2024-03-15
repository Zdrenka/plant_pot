#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>
#include <random>
#include <WiFi.h>
#include <string>

#include "happy.h"
#include "sad.h"
#include "sad-up.h"
#include "looking-left.h"
#include "looking-right.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char *ssid = "";
const char *password = "";
// Set web server port number to 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 16
#define OLED_CS 5
#define OLED_RESET 17

#include <GyverOLED.h>

const uint8_t *is_happy = happy;
const uint8_t *is_sad = sad;
const uint8_t *is_looking_left = left;
const uint8_t *is_looking_right = right;
const uint8_t *is_sad_up = sad_up;

#define AOUT_PIN 35
#define THRESHOLD 150

const int DRY = 2540;
const int WET = 910;

GyverOLED<SSH1106_128x64> oled;

unsigned long previousMillis = 0;
const long interval = 1000;

int getCurrentState()
{
  int value = analogRead(AOUT_PIN); // read the analog value from sensor
  int percentage = map(value, DRY, WET, 0, 100);
  return percentage;
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String htmlContent = "<!DOCTYPE html>\n"
              "<html>\n"
              "<head>\n"
              "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
              "  <link rel=\"icon\" href=\"data:,\">\n"
              "  <style>\n"
              "    body { font-family: 'Helvetica', sans-serif; margin: 0; padding: 0; background-color: #f0f8ff; background-image: url('https://www.example.com/background.jpg'); background-size: cover; color: #333; text-align: center; }\n"
              "    h1 { color: #336633; }\n"
              "    h2 { color: #2e8b57; }\n"
              "    .content { background-color: rgba(255, 255, 255, 0.8); margin: 20px auto; padding: 20px; border-radius: 10px; width: 90%; max-width: 400px; }\n"
              "    .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }\n"
              "    .button2 { background-color: #555555; }\n"
              "    .icon { width: 200px; height: auto; }\n"
              "  </style>\n"
              "</head>\n"
              "<body>\n"
              "  <div class=\"content\">\n"
              "    <h1>Karls Office Plant</h1>\n"
              "    <img src=\"https://d1nhio0ox7pgb.cloudfront.net/_img/o_collection_png/green_dark_grey/256x256/plain/plant.png\" alt=\"Plant Icon\" class=\"icon\">\n"
              "    <h2 id=\"moisture\">Moisture is at --%</h2>\n"
              "  </div>\n"
              "\n"
              "  <script>\n"
              "    var evtSource = new EventSource(\"/events\");\n"
              "    evtSource.addEventListener('new_state', function(e) {\n"
              "      document.getElementById(\"moisture\").innerHTML = \"Moisture is at \" + e.data + \"%\";\n"
              "    });\n"
              "  </script>\n"
              "</body>\n"
              "</html>\n";


  request->send(200, "text/html", htmlContent); });

  events.onConnect([](AsyncEventSourceClient *client)
                   {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // Send the current state immediately after connection
    client->send(String(getCurrentState()).c_str(), NULL, millis(), 10000); });
  server.addHandler(&events);

  server.begin();
  oled.init();
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // Save the last time you blinked the LED
    int percentage = getCurrentState();
    events.send(String(percentage).c_str(), "new_state", millis());
    Serial.println(percentage);
    delay(3000);
    oled.clear();
    oled.autoPrintln(true);
    oled.setScale(4);
    oled.setCursor(percentage >= 100 ? 20 : 35, 2);
    oled.print(String(percentage) + "%");
    oled.update();
  }
}
