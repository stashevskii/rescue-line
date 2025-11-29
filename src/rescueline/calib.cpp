#include <Arduino.h>
#include <EEPROM.h>
#include "move.hpp"
#include "calib.hpp"
#include "config.hpp"
#include "lfr.hpp"
#include "tcs34725.hpp"

static constexpr int calibSpeed = 90;
static constexpr int calibTime = 11000;

int calibMax[4], calibMin[4];
long tcsMin[3], tcsMax[3];

void calibrate() {
  for (int i = 0; i < N; ++i) {
    calibMin[i] = 1000;
    calibMax[i] = 0;
  }
  unsigned long tmr = millis();
  while (millis() <= tmr + calibTime) {
    driveFront(-calibSpeed, calibSpeed);
    driveBack(-calibSpeed, calibSpeed);
    readSensors();
    for (int i = 0; i < N; ++i) {
      calibMax[i] = max(calibMax[i], lineValues[i]);
      calibMin[i] = min(calibMin[i], lineValues[i]);
    }
  }
  driveFront(0, 0);
  driveBack(0, 0);
  int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, calibMax[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, calibMin[i]);
    adr += sizeof(int);
  }
}

void calibrateCS() {
  for (int i = 0; i < 3; ++i) {
    tcsMin[i] = 100000;
    tcsMax[i] = 0;
  }
  unsigned long tmr = millis();
  uint16_t arr[3];
  while (millis() <= tmr + calibTime) {
    driveFront(-45, 45);
    driveBack(-45, 45);
    getRawRGB(&arr[0], &arr[1], &arr[2]);
    for (int i = 0; i < 3; ++i) {
      tcsMax[i] = max(tcsMax[i], arr[i]);
      tcsMin[i] = min(tcsMin[i], arr[i]);
    }
  }
  driveFront(0, 0);
  driveBack(0, 0);
  /*int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, calibMax[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, calibMin[i]);
    adr += sizeof(int);
  }*/
}
