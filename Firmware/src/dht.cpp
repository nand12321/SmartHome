#include "dht.h"

DHT dht(DHTPIN, DHTTYPE);

void dhtSetup() {
    dht.begin();
}

float getTemperature() {
    float t = dht.readTemperature();
    if (isnan(t)) {
        Serial.println("can't read temperature");
    }
}

float getHumidity() {
    float h = dht.readHumidity();
    if (isnan(h)) {
        Serial.println("can't read humidty");
    }
}