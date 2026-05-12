/*
  dashboard-rc.ino
  Test firmware minimal untuk memastikan GitHub Actions berhasil compile
  Board: ESP32-C6 Dev Module
*/

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println(" Dashboard RC ESP32-C6 Started ");
  Serial.println("================================");
}

void loop() {
  Serial.println("System running...");
  delay(1000);
}