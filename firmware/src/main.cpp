#include <Arduino.h>
#include "dht.h"


void setup() {
  dht_setup();
}

void loop() {
  dht_update();
}