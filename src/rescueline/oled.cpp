#include <U8g2lib.h>
#include "oled.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void initOLED() {
  u8g2.begin(/*Select=*/ 12, /*Right/Next=*/ 15, /*Left/Prev=*/ 14, /*Up=*/ A0, /*Down=*/ A3, /*Home/Cancel=*/ 8); // Arduboy 10 (Production)
  u8g2.setFont(u8g2_font_6x12_tr);
}

void printArray(int* arr, int size, int len, int m, int k) {
  for (int i = 0; i < size; ++i) {
    char bf[len];
    itoa(arr[i], bf, len);
    u8g2.drawStr(m, i + k, bf);
  }
}
