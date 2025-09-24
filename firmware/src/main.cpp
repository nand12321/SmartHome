#include <Arduino.h>
#include "dht.h"
#include <WiFi.h>
// #include "Servo.h"


Servo doorServo;

const char* ssid = "CoolWiFi";
const char* password = "123456789";

struct Controller {
  int pin;
  bool status;
};

Controller mainLightController{21, false};
Controller roofLightController{22, false};
Controller doorController{19, false};

WiFiServer wifiServer(80);

void setupWIFI(const char* ssid, const char* password);
void serverLoop();
char* readStringUntil(char c, char str[], int n);

void setup() {
  Serial.begin(9600);

  doorServo.attach(doorController.pin);
  dhtSetup();
  setupWIFI(ssid, password);

  pinMode(mainLightController.pin, OUTPUT);
  pinMode(roofLightController.pin, OUTPUT);
  pinMode(doorController.pin, OUTPUT);
}

void loop() {
  // float temperature = getTemperature();
  // float humidity = getHumidity();

  if (mainLightController.status = true) {
    digitalWrite(mainLightController.pin, HIGH);
  }
  else {
    digitalWrite(mainLightController.pin, LOW);
  }
  if (roofLightController.status = true) {
    digitalWrite(roofLightController.pin, HIGH);
  }
  else {
    digitalWrite(roofLightController.pin, LOW);
  }
  if (doorController.status = true) {
    doorServo.write(90);
  }
  else {
    doorServo.write(0);
  }

  serverLoop();
}

void setupWIFI(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

void serverLoop() {
    WiFiClient client = wifiServer.available();
    if (client) {
      Serial.println("Client connected!");
      while (client.connected()) {

        if (client.available()) {
          String req = client.readStringUntil('\n');
          req.trim(); // remove \r\n

          int sep = req.indexOf(':');
          String command = req.substring(0, sep);
          String value   = req.substring(sep + 1);

          Serial.print("Command: ");
          Serial.println(command);
          Serial.print("Value: ");
          Serial.println(value);

          if (command == "MAIN_LIGHT") {
              mainLightController.status = (value == "ON") ? true : false;
          }
          else if (command == "ROOF_LIGHT") {
              roofLightController.status = (value == "ON") ? true : false;
          }
          else if (command == "DOOR") {
              doorController.status = (value == "ON") ? true : false;
          }

          client.println("ESP32 says: " + req);
        }

      // Serial.print("Received: ");
      // Serial.println(req);

      // client.println("ESP32 says: " + req);
    }
  }
}

