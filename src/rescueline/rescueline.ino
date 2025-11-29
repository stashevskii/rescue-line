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
  Serial.begin(BAUD_RATE);
  for (int i : OUTPUT_MODE) pinMode(i, OUTPUT);
  for (int i : BUTTONS) pinMode(i, INPUT_PULLUP);
  digitalWrite(30, HIGH);
  initOLED();
  getCalibEeprom(0, lineCalibMax, lineCalibMin, 4);
  getCalibEeprom(8 * sizeof(int), tcsCalibMax, tcsCalibMin, 3);
}

void run() {
  digitalWrite(LED3, HIGH);
  LFR(150);
  digitalWrite(LED3, LOW);
}

void calibLine() {
  displayText(25, 35, "Line calibration", true);
  calibrateLine();
  displayText(10, 10, "min:", true);
  displayArray(tcsCalibMin, 3, 20, 10);
  displayText(70, 10, "max:", true);
  displayArray(tcsCalibMax, 3, 80, 10);
  while (digitalRead(BTN_SET));
  delay(250);
}

void calibTcs() {
  displayText(25, 35, "Calibration TCS34725", true);
  calibrateTcs();
  displayText(10, 10, "min:", true);
  displayArray(tcsCalibMin, 3, 20, 10);
  displayText(70, 10, "max:", true);
  displayArray(tcsCalibMax, 3, 80, 10);
  while (digitalRead(BTN_SET));
  delay(250);
}

void displayTcs() {
  uint16_t a[3];
  while (digitalRead(BTN_PLUS)) {
    getRGBMap(&a[0], &a[1], &a[2]);
    u8g2.clearBuffer();
    displayArray(a, 3, 10, 10);
  }
}

const char *string_list = 
  "Line calib\n"
  "CS calib\n"
  "Read CS\n"
  "Run";

void loop() {
  static uint8_t curr;
  curr = u8g2.userInterfaceSelectionList(
    "Menu",
    curr, 
    string_list
  );
  if (curr == 1) calibLine();
  else if (curr == 2) calibTcs();
  else if (curr == 3) displayTcs();
  else if (curr == 4) run();
}
