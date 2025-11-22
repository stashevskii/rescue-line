#pragma once

#include <U8x8lib.h>

extern U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

void initOLED();
void printArray(int* arr, int size, int len, int m, int k);
