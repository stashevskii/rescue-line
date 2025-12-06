#pragma once

#include "Adafruit_TCS34725.h"

extern Adafruit_TCS34725 tcs;
extern const uint8_t RIGHT_TCS, LEFT_TCS;

void getRawRGB(uint16_t *r, uint16_t *g, uint16_t *b, uint8_t ch);
void getRGBMap(uint16_t *r, uint16_t *g, uint16_t *b, uint8_t ch);
