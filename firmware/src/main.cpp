#include <Arduino.h>
#include "DHT.h"


#define DHTPIN 18

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(2000);
  Serial.begin(9600);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t)) {
    Serial.println("something went wrong");
  }
  Serial.print("T: ");
  Serial.print(t);
  Serial.print(" C");
  Serial.print(" H: ");
  Serial.print(h);
  Serial.println(" %");

  delay(2000);
}