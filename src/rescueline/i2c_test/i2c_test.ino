#include <Wire.h>
#include "../config.hpp"

constexpr int BAUD_RATE = 19200;
constexpr int CHAR_BUF = 128;
<<<<<<< HEAD
constexpr double KP = 0.7;
=======
constexpr int KP = 0.8;

void standBy() {
  pinMode(30, OUTPUT);
  digitalWrite(30, HIGH);
}
>>>>>>> 4a0412d457596e574989d605e8f184c62c8e8780

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

<<<<<<< HEAD
  double err = atoi(buff);
  err *= KP;
  
=======
  int err = atoi(buff);
  err *= KP;

>>>>>>> 4a0412d457596e574989d605e8f184c62c8e8780
}