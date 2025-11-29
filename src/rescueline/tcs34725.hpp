#pragma once

#include "Adafruit_TCS34725.h"

extern Adafruit_TCS34725 tcs;

void getRawRGB(uint16_t *r, uint16_t *g, uint16_t *b);
void getRGBMap(uint16_t *r, uint16_t *g, uint16_t *b);