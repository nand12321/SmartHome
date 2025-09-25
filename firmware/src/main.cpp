#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include "dht.h"

Servo doorServo;

const char* ssid = "CoolWiFi";
const char* password = "123456789";

const int MAIN_LIGHT_PIN = 21;
const int ROOF_LIGHT_PIN = 22;
const int DOOR_PIN       = 19;

bool mainLightStatus = false;
bool roofLightStatus = false;
bool doorStatus = false;

WiFiServer wifiServer(80);

void setupWIFI() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.println("ESP32 IP: " + WiFi.localIP().toString());
    wifiServer.begin();
}

void setup() {
    Serial.begin(9600);

    doorServo.attach(DOOR_PIN);
    dhtSetup();

    pinMode(MAIN_LIGHT_PIN, OUTPUT);
    pinMode(ROOF_LIGHT_PIN, OUTPUT);
    pinMode(DOOR_PIN, OUTPUT);

    setupWIFI();
}

void loop() {
    WiFiClient client = wifiServer.available();
    if (client) {
        Serial.println("Client connected!");
        while (client.connected()) {
            if (client.available()) {
                String req = client.readStringUntil('\n');
                req.trim();

                int sep = req.indexOf(':');
                if (sep == -1) continue;

                String command = req.substring(0, sep);
                String value   = req.substring(sep + 1);

                Serial.print("Command: ");
                Serial.println(command);
                Serial.print("Value: ");
                Serial.println(value);

                if (command == "MAIN_LIGHT") {
                    mainLightStatus = (value == "ON");
                    digitalWrite(MAIN_LIGHT_PIN, mainLightStatus ? HIGH : LOW);
                }
                else if (command == "ROOF_LIGHT") {
                    roofLightStatus = (value == "ON");
                    digitalWrite(ROOF_LIGHT_PIN, roofLightStatus ? HIGH : LOW);
                }
                else if (command == "DOOR") {
                    doorStatus = (value == "ON");
                    doorServo.write(doorStatus ? 90 : 0);
                }

                client.println("ESP32 says: " + req);
            }
        }
        client.stop();
        Serial.println("Client disconnected.");
    }
}
