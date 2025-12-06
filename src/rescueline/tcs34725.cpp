#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "tcs34725.hpp"
#include "tca9548a.hpp"
#include "calib.hpp"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_16X);

const uint8_t RIGHT_TCS = 1, LEFT_TCS = 2;

void getRawRGB(uint16_t *r, uint16_t *g, uint16_t *b, uint8_t ch) {
  selectChannel(ch);
  uint16_t _;
  tcs.getRawData(r, g, b, &_);
}

void getRGBMap(uint16_t *r, uint16_t *g, uint16_t *b, uint8_t ch) {
  getRawRGB(r, g, b, ch);
  uint16_t* tcsCalibMax = ch == 1 ? tcsCalibMaxR : tcsCalibMaxL;
  uint16_t* tcsCalibMin = ch == 1 ? tcsCalibMinR : tcsCalibMinL;
  *r = constrain(map(*r, 0, 65000, 0, 255), 0, 255);
  *g = constrain(map(*g, 0, 65000, 0, 255), 0, 255);
  *b = constrain(map(*b, 0, 65000, 0, 255), 0, 255);
}
