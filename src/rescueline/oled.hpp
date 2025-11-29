#pragma once

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void initOLED();

template<typename T>
void printArray(T* arr, int size, int x, int y, int len = 10) {
  for (int i = 0; i < size && i < len; ++i) {
    u8g2.setCursor(x, y + (i * 12));
    u8g2.print(arr[i]);
  }
}