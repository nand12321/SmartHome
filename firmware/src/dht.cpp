#include "dht.h"

DHT dht(DHTPIN, DHTTYPE);

void dht_setup() {
    Serial.begin(9600);
    dht.begin();
    delay(2000);
    Serial.begin(9600);
}

void dht_update() {
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