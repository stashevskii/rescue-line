#include <Arduino.h>
#include <EEPROM.h>
#include "move.hpp"
#include "calib.hpp"
#include "config.hpp"
#include "lfr.hpp"
#include "tcs34725.hpp"
#include "oled.hpp"

static constexpr int calibSpeed = 90;
static constexpr int calibTime = 11000;

int lineCalibMax[4], lineCalibMin[4];
uint16_t tcsCalibMinR[3], tcsCalibMaxR[3];
uint16_t tcsCalibMinL[3], tcsCalibMaxL[3];

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

static void tcsCalib(uint16_t *arr, uint16_t *v, uint8_t ch) {
  getRawRGB(&arr[0], &arr[1], &arr[2], ch);
  for (int i = 0; i < 3; ++i) {
    v[i] = arr[i];
  }
}

void calibrateTcs() {
  for (int i = 0; i < 3; ++i) {
    tcsCalibMinR[i] = tcsCalibMinL[i] = 65535;
    tcsCalibMaxR[i] = tcsCalibMaxL[i] = 0;
  }
  uint16_t arr[3];
  displayText(30, 30, "WHITE", true);
  while (digitalRead(BTN_SET));
  tcsCalib(arr, tcsCalibMaxR, RIGHT_TCS);
  tcsCalib(arr, tcsCalibMaxL, LEFT_TCS);
  delay(200);
  displayText(30, 30, "BLACK", true);
  while (digitalRead(BTN_SET));
  tcsCalib(arr, tcsCalibMinR, RIGHT_TCS);
  tcsCalib(arr, tcsCalibMinL, LEFT_TCS);
  putEeprom(8 * sizeof(int), tcsCalibMaxR, tcsCalibMinR, 3);
  putEeprom(14 * sizeof(int), tcsCalibMaxL, tcsCalibMinL, 3);
}
