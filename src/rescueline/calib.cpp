#include "calib.hpp"
#include <Arduino.h>
#include <EEPROM.h>

void calibrate() {
  int maxS[N], minS[N];
  for (int i = 0; i < N; ++i) {
    minS[i] = 1000;
    maxS[i] = 0;
  }
  unsigned long tmr = millis();
  while (millis() <= tmr + 5000) {
    readSensors();
    for (int i = 0; i < N; ++i) {
      maxS[i] = max(maxS[i], lineValues[i]);
      minS[i] = min(minS[i], lineValues[i]);
    }
  }
  int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, maxS[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, minS[i]);
    adr += sizeof(int);
  }
}

void readCalibValues(int* mx, int* mn) {
  int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.get(adr, mx[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.get(adr, mn[i]);
    adr += sizeof(int);
  }
}