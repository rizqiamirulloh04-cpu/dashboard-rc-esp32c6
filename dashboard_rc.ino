#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Dashboard RC ESP32-C6 started");
}

void loop() {
  Serial.println("Running...");
  delay(1000);
}