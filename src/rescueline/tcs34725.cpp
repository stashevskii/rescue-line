#include "Adafruit_TCS34725.h"
#include "tcs34725.hpp"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_16X);

void getRawRGB(uint16_t *r, uint16_t *g, uint16_t *b) {
  uint16_t _;
  tcs.getRawData(r, g, b, &_);
}
