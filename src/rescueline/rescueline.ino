#include <Arduino.h>
#include <EEPROM.h>
#include <U8g2lib.h>
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
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(30, HIGH);
  Serial.begin(BAUD_RATE);
  pinMode(BTN_SET, INPUT_PULLUP);
  pinMode(BTN_PLUS, INPUT_PULLUP);
  initOLED();
  int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.get(adr, calibMax[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.get(adr, calibMin[i]);
    adr += sizeof(int);
  }
}

void testDrive() {
  while (true) {
    driveFront(0, 100);
    driveBack(0, 100);
  }
}

void loop() {
  if (!digitalRead(BTN_SET)) {
    u8g2.clear();
    calibrate();
    u8g2.drawStr(0, 1, "min");
    printArray(calibMin, N, 10, 0, 10);
    u8g2.drawStr(8, 1, "max");
    printArray(calibMax, N, 10, 8, 10);
    u8g2.sendBuffer();
    delay(6000);
  }
  if (!digitalRead(BTN_PLUS)) {
    digitalWrite(LED3, HIGH);
    LFR(170);
    digitalWrite(LED3, HIGH);
  }
}
