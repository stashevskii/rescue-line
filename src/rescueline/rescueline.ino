#include <EEPROM.h>
#include <U8x8lib.h>
#include "move.hpp"
#include "config.hpp"
#include "oled.hpp"
#include "lfr.hpp"
#include "calib.hpp"

void setup() {
  for (int i = 0; i < 12; i++) {
    pinMode(MOTOR_PINS[i], OUTPUT);
  }
  pinMode(30, OUTPUT);
  digitalWrite(30, HIGH);
  Serial.begin(BAUD_RATE);
  pinMode(BTN_SET, INPUT_PULLUP);
  initOLED();
}

void testDrive() {
  while (true) {
    driveFront(0, 100);
    driveBack(0, 100);
  }
}

void loop() {
  if (!digitalRead(BTN_SET)) {
    u8x8.clear();
    calibrate();
    int maxS[N], minS[N];
    fetchCalib(maxS, minS);
    u8x8.drawString(0, 1, "min");
    printArray(minS, N, 10, 0, 10);
    u8x8.drawString(8, 1, "max");
    printArray(maxS, N, 10, 8, 10);
    delay(10000);
  }
}
