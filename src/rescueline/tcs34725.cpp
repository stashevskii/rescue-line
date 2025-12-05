#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "tcs34725.hpp"
#include "calib.hpp"

static constexpr int TCA9548A = 0x70;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_16X);

void selectChan(uint8_t bus) {
  if (bus > 7) return;
  Wire.beginTransmission(TCA9548A);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void getRawRGB(uint16_t *r, uint16_t *g, uint16_t *b) {
  uint16_t _;
  tcs.getRawData(r, g, b, &_);
}

void getRGBMap(uint16_t *r, uint16_t *g, uint16_t *b) {
  getRawRGB(r, g, b);
  *r = constrain(map(*r, tcsCalibMin[0], tcsCalibMax[0], 0, 255), 0, 255);
  *g = constrain(map(*g, tcsCalibMin[1], tcsCalibMax[1], 0, 255), 0, 255);
  *b = constrain(map(*b, tcsCalibMin[2], tcsCalibMax[2], 0, 255), 0, 255);
}