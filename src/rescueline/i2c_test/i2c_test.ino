#include <Wire.h>
#include "../config.hpp"

constexpr int BAUD_RATE = 19200;
constexpr int CHAR_BUF = 128;
constexpr int KP = 0.8;

void standBy() {
  pinMode(30, OUTPUT);
  digitalWrite(30, HIGH);
}

void setup() {
  standBy();
  Serial.begin(BAUD_RATE);
  Wire.begin();
}

void loop() {
  int32_t temp = 0;
  char buff[CHAR_BUF] = {0};

  Wire.requestFrom(0x12, 2);
  if (Wire.available() == 2) {

    temp = Wire.read() | (Wire.read() << 8);

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

  int err = atoi(buff);
  err *= KP;

}