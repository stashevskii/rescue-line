#include <Arduino.h>

String getDirection() {
  String received_string = "";
  if (Serial2.available()) {
    String received_string = Serial2.readStringUntil('\n');
    return received_string;
  }
}