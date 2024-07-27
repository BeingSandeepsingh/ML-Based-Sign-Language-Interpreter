#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "ESPAsyncWebServer.h"

#define WIFI_SSID "Redmi Note 12 pro 5G"
#define WIFI_PASSWORD "12345678"

// MPU6050 mpu;

unsigned long sendDataPrevMillis = 0;
bool signupOK = true;

const int flexPin1 = 34;  // Analog input pin for Flex Sensor 1
const int flexPin2 = 35;  // Analog input pin for Flex Sensor 2
const int flexPin3 = 33;  // Analog input pin for Flex Sensor 3
const int flexPin4 = 32;  // Analog input pin for Flex Sensor 4
// const int flexPin5 = 4;  // Analog input pin for Flex Sensor 5 

AsyncWebServer server(80);

char serial[512];

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", serial);
  });

  // Wire.begin(21, 22);  // SDA pin 21, SCL pin 22
  // mpu.initialize();

  // // Calibrate the accelerometer
  // mpu.CalibrateGyro();
}

void loop() {

  Serial.println(WiFi.localIP());
  if (signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Read flex sensor values
    float flexValue1 = analogRead(flexPin1);
    float flexValue2 = analogRead(flexPin2);
    float flexValue3 = analogRead(flexPin3);
    float flexValue4 = analogRead(flexPin4);

    Serial.println(flexValue1);
    Serial.println(flexValue2);
    Serial.println(flexValue3);
    Serial.println(flexValue4);

    // float flexValue5 = analogRead(flexPin5);

    // Read accelerometer data
    // int16_t ax, ay, az;
    // mpu.getAcceleration(&ax, &ay, &az);

    WiFiClient wifiClient;
    HTTPClient client;
    client.begin(wifiClient, "http://192.168.76.75:5000/predict");
    client.addHeader("Content-Type", "application/json");
    const size_t capacity = JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> doc;
    doc["flex_1"] = flexValue1;
    doc["flex_2"] = flexValue2;
    doc["flex_3"] = flexValue3;
    doc["flex_4"] = flexValue4;
    
    // doc["flex_5"] = flexValue5;
    // doc["ACCx"] = 456;
    // doc["ACCy"] = 9.56;
    // doc["ACCz"] = 8.32;

    String jsonStr;
    serializeJson(doc, jsonStr);
    int statusCode = client.POST(jsonStr);
    if (statusCode > 0) {
      Serial.print("HTTP status code: ");
      Serial.println(statusCode);
      String payload = client.getString();
      Serial.println("Response: ");
      Serial.println(payload);
      const size_t capacity2 = JSON_OBJECT_SIZE(4);
      StaticJsonDocument<capacity2> doc2;
      doc2["flex_1"] = flexValue1;
      doc2["flex_2"] = flexValue2;
      doc2["flex_3"] = flexValue3;
      doc2["flex_4"] = flexValue4;
      // doc2["flex_5"] = flexValue5;
      // doc2["ACCx"] = 456;
      // doc2["ACCy"] = 9.56;
      // doc2["ACCz"] = 8.32;
      doc2["message"] = payload;
      serializeJson(doc2, Serial);
    } else {
      Serial.print("Error: ");
      Serial.println(statusCode);
    }
  }

  delay(5000);  // Add delay at the end of loop
}