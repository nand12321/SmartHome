#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(10);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    Serial.println(cmd);

    if (cmd == "OPEN") {
        servo.write(90);
    } 
    else if (cmd == "CLOSE") {
        servo.write(0);
    }
  }
}
