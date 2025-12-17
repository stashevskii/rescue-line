#include <Arduino.h>

char getDirection() {
  if (Serial2.available()) {
    char response = Serial2.read();
    return response;
  }
  return '4';
}