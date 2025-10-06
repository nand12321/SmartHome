#pragma once
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

extern DHT dht;

void dhtSetup();
float getTemperature();
float getHumidity();