#include <U8g2lib.h>
#include "oled.hpp"
#include "config.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void initOLED() {
  u8g2.begin(BTN_SET, BTN_PLUS, BTN_MINUS);
  u8g2.setFont(u8g2_font_6x12_tr);
}

void displayText(int x, int y, const char *s, bool clear = false) {
  if (clear) u8g2.clear();
  u8g2.drawStr(x, y, s);
  u8g2.sendBuffer();
}