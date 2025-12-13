#include <Arduino.h>
#include <EEPROM.h>
#include "move.hpp"
#include "calib.hpp"
#include "config.hpp"
#include "lfr.hpp"
#include "oled.hpp"

static constexpr int calibSpeed = 90;
static constexpr int calibTime = 11000;

int lineCalibMax[4], lineCalibMin[4];

static void drv(int s = calibSpeed) {
  driveFront(-s, s);
  driveBack(-s, s);
}

template<typename T>
static void putEeprom(int adr, T* mx, T* mn, int size) {
  for (int i = 0; i < size; i++) {
    EEPROM.put(adr, mx[i]);
    adr += sizeof(T);
  }
  for (int i = 0; i < size; i++) {
    EEPROM.put(adr, mn[i]);
    adr += sizeof(T);
  }
}

void calibrateLine() {
  for (int i = 0; i < 4; ++i) {
    lineCalibMin[i] = 1000;
    lineCalibMax[i] = 0;
  }
  unsigned long tmr = millis();
  while (millis() <= tmr + calibTime) {
    drv();
    readSensors();
    for (int i = 0; i < 4; ++i) {
      lineCalibMax[i] = max(lineCalibMax[i], lineVals[i]);
      lineCalibMin[i] = min(lineCalibMin[i], lineVals[i]);
    }
  }
  drv(0);
  putEeprom(0, lineCalibMax, lineCalibMin, 4);
}
