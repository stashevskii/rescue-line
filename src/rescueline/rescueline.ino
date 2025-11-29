#include <Arduino.h>
#include <EEPROM.h>
#include <U8g2lib.h>
#include "move.hpp"
#include "config.hpp"
#include "oled.hpp"
#include "lfr.hpp"
#include "calib.hpp"
#include "tcs34725.hpp"

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
  for (int i = 0; i <4; i++) {
    EEPROM.get(adr, lineCalibMax[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i <4; i++) {
    EEPROM.get(adr, lineCalibMin[i]);
    adr += sizeof(int);
  }
}

void run() {
  digitalWrite(LED3, HIGH);
  LFR(150);
  digitalWrite(LED3, LOW);
}

void calib() {
  u8g2.clear();
  u8g2.drawStr(25, 25, "calibration");
  u8g2.sendBuffer();
  calibrateLine();
  u8g2.clear();
  u8g2.drawStr(10, 10, "min:");
  printArray(lineCalibMin,4, 20, 10);
  u8g2.drawStr(70, 10, "max:");
  printArray(lineCalibMax,4, 80, 10);
  u8g2.sendBuffer();
  while (digitalRead(BTN_SET));
  delay(300);
}

void calibCS() {
  u8g2.clear();
  u8g2.drawStr(25, 25, "calibration CS");
  u8g2.sendBuffer();
  calibrateTcs();
  u8g2.clear();
  u8g2.drawStr(10, 10, "min:");
  printArray(tcsCalibMin, 3, 20, 10);
  u8g2.drawStr(70, 10, "max:");
  printArray(tcsCalibMax, 3, 80, 10);
  u8g2.sendBuffer();
  while (digitalRead(BTN_SET));
  delay(300);
}

void readTcs() {
  uint16_t a[3];
  while (digitalRead(BTN_PLUS)) {
    getRGBMap(&a[0], &a[1], &a[2]);
    u8g2.clearBuffer();
    printArray(a, 3, 10, 10, 20);
    u8g2.sendBuffer();
  }
}

const char *string_list = 
  "Line calib\n"
  "CS calib\n"
  "Read CS\n"
  "Run";

void loop(void) {
  static uint8_t current_selection;
  current_selection = u8g2.userInterfaceSelectionList(
    "Menu",
    current_selection, 
    string_list
  );
  if (current_selection == 1) calib();
  else if (current_selection == 2) calibCS();
  else if (current_selection == 3) readTcs();
  else if (current_selection == 4) run();
}
