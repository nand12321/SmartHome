#include <Arduino.h>
#include <WiFi.h>
#include "dht.h"

const char* ssid = "Wi-Fi SSID";
const char* password = "password";

const int MAIN_LIGHT_PIN = 21;
const int ROOF_LIGHT_PIN = 22;

bool mainLightStatus = false;
bool roofLightStatus = false;
bool doorStatus = false;

WiFiServer wifiServer(80);

void setupWIFI()
{
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.println("ESP32 IP: " + WiFi.localIP().toString());
    wifiServer.begin();
   //  digitalWrite(2, HIGH);
}

void setup()
{
    Serial.begin(9600);

    // Communicating with the arduino to control the door's servo 
    Serial2.begin(9600, SERIAL_8N1, 16, 17);

    pinMode(MAIN_LIGHT_PIN, OUTPUT);
    pinMode(ROOF_LIGHT_PIN, OUTPUT);
    // pinMode(2, OUTPUT);

    setupWIFI();
}

void loop()
{
    Serial2.println("ON");
    WiFiClient client = wifiServer.available();
    if (client) {
        Serial.println("Client connected!");
        while (client.connected()) {
            if (client.available()) {
                String req = client.readStringUntil('\n');

                // Serial.print("req: ");
                // Serial.println(req);

                if (req == "MAIN_LIGHT") {
                    mainLightStatus = !mainLightStatus;

                    digitalWrite(MAIN_LIGHT_PIN, mainLightStatus ? HIGH : LOW);
                }
                else if (req == "ROOF_LIGHT") {
                    roofLightStatus = !roofLightStatus;

                    digitalWrite(ROOF_LIGHT_PIN, roofLightStatus ? HIGH : LOW);
                }
                else if (req == "DOOR") {
                    doorStatus = !doorStatus;

                    if (doorStatus) {
                        Serial.println("OPEN");
                    } else {
                        Serial.println("CLOSE");
                    }
                    
                    // Serial.print("Door status: ");
                    // Serial.println(doorStatus ? "OPEN" : "CLOSE");
                }
            }
            // client.println("temprature: " + String(getTemperature()) + " humidity: " + String(getHumidity()));
            // client.println("34,50");
        }
        client.stop();
        Serial.println("Client disconnected.");
    }
}
