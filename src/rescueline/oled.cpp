#include <U8g2lib.h>
#include "oled.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void initOLED() {
  u8g2.begin(12, 15, 14, 15, 14, 8);
  u8g2.setFont(u8g2_font_6x12_tr);
}
