#include <U8x8lib.h>
#include "oled.hpp"

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void initOLED() {
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setInverseFont(0);
}

void printArray(int* arr, int size, int len, int m, int k) {
  for (int i = 0; i < size; ++i) {
    char bf[len];
    itoa(arr[i], bf, len);
    u8x8.drawString(m, i + k, bf);
  }
}
