#pragma once
#include "DHT.h"

#define DHTPIN 18
#define DHTTYPE DHT22

extern DHT dht;

void dht_setup();

void dht_update();