#pragma once
#include "DHT.h"

#define DHTPIN 18
#define DHTTYPE DHT22

extern DHT dht;

void dhtSetup();
float getTemperature();
float getHumidity();