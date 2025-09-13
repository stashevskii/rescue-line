#include <Wire.h>

constexpr int BAUD_RATE = 19200
constexpr int CHAR_BUF = 128

void setup() {
  Serial.begin(BAUD_RATE);
  Wire.begin();
  delay(1000);
}

void loop() {
  int32_t temp = 0;
  char buff[CHAR_BUF] = {0};

  Wire.requestFrom(0x12, 2);
  if (Wire.available() == 2) {

    temp = Wire.read() | (Wire.read() << 8);
    delay(1);

    Wire.requestFrom(0x12, temp);
    if (Wire.available() == temp) {
      temp = 0;
      while(Wire.available()) buff[temp++] = Wire.read();

    } else {
      while(Wire.available()) Wire.read();
    }
  } else {
    while (Wire.available()) Wire.read();
  }

  Serial.println(buff);
  delay(1);
}