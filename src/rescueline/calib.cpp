#include <Arduino.h>
#include <EEPROM.h>
#include "move.hpp"
#include "calib.hpp"
#include "config.hpp"
#include "lfr.hpp"
#include "tcs34725.hpp"

static constexpr int calibSpeed = 90;
static constexpr int calibTime = 11000;

int lineCalibMax[4], lineCalibMin[4];
uint16_t tcsCalibMin[3], tcsCalibMax[3];

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

void calibrateTcs() {
  for (int i = 0; i < 3; ++i) {
    tcsCalibMin[i] = 65535;
    tcsCalibMax[i] = 0;
  }
  unsigned long tmr = millis();
  uint16_t arr[3];
  while (millis() <= tmr + calibTime) {
    drv();
    getRawRGB(&arr[0], &arr[1], &arr[2]);
    for (int i = 0; i < 3; ++i) {
      tcsCalibMax[i] = max(tcsCalibMax[i], arr[i]);
      tcsCalibMin[i] = min(tcsCalibMin[i], arr[i]);
    }
  }
  drv(0);
  putEeprom(8 * sizeof(int), tcsCalibMax, tcsCalibMin, 3);
}
