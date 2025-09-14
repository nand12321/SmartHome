#include <WiFi.h>
#include <PubSubClient.h>
#include "dht.h"

const char* ssid = "CoolWiFi";
const char* password = "123456789";

const char* mqtt_server = "10.125.43.102";
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char mag[50];
int value = 0;

struct Controller {
  int pin;
  bool status;
};

Controller mainLightController{21, false};
Controller roofLightController{22, false};
Controller doorController{19, false};

void setupWIFI();
void callback(char* topic, byte* message, unsigned int length);
void reconnect();

void setup() {
  Serial.begin(9600);
  dhtSetup();
  setupWIFI();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(mainLightController.pin, OUTPUT);
  pinMode(roofLightController.pin, OUTPUT);
  pinMode(doorController.pin, OUTPUT);
}

void setupWIFI() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("smarthome/mainlight");
      client.subscribe("smarthome/rooflight");
      client.subscribe("smarthome/door");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    float temperature = getTemperature();
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("smarthome/temperature", tempString);

    // Convert the value to a char array
    float humidity = getHumidity();
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("smarthome/humidity", humString);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (topic == "smarthome/mainlight") {
    Serial.println("Main light control");
    if(messageTemp == "ON"){
      mainLightController.status = true;
    } else if(messageTemp == "OFF"){
      mainLightController.status = false;
    }
    digitalWrite(mainLightController.pin, mainLightController.status);
  }
  else if (String(topic) == "smarthome/rooflight") {
    if(messageTemp == "ON"){
      roofLightController.status = true;
    } else if(messageTemp == "OFF"){
      roofLightController.status = false;
    }
    digitalWrite(roofLightController.pin, roofLightController.status);
  }
  else if (String(topic) == "smarthome/door") {
    if(messageTemp == "OPEN"){
      doorController.status = true;
    } else if(messageTemp == "CLOSE"){
      doorController.status = false;
    }
    digitalWrite(doorController.pin, doorController.status);
  }
}