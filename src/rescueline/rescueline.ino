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

void run() {
  digitalWrite(LED3, HIGH);
  LFR(150);
  digitalWrite(LED3, LOW);
}

void calib() {
  u8g2.clear();
  u8g2.drawStr(25, 25, "calibration");
  calibrate();
  u8g2.clear();
  u8g2.drawStr(10, 10, "min:");
  printArray(calibMin, N, 10, 10);
  u8g2.drawStr(70, 10, "max:");
  printArray(calibMax, N, 70, 10);
  u8g2.sendBuffer();
  while (digitalRead(BTN_SET));
  delay(300);
}

const char *string_list = 
  "Calibration\n"
  "Run";

void loop(void) {
  static uint8_t current_selection;
  current_selection = u8g2.userInterfaceSelectionList(
    "Menu",
    current_selection, 
    string_list
  );
  if (current_selection == 1) calib();
  else if (current_selection == 2) run();
}
