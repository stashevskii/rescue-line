#include <Arduino.h>
#include <EEPROM.h>
#include <U8g2lib.h>
#include "move.hpp"
#include "config.hpp"
#include "oled.hpp"
#include "lfr.hpp"
#include "calib.hpp"
#include "cam.hpp"

void setup() {
  Serial.begin(BAUD_RATE);
  Serial2.begin(BAUD_RATE);
  for (int i : OUTPUT_MODE) pinMode(i, OUTPUT);
  for (int i : BUTTONS) pinMode(i, INPUT_PULLUP);
  digitalWrite(30, HIGH);
  initOLED();
  getCalibEeprom(0, lineCalibMax, lineCalibMin, 4);
}

void run() {
  digitalWrite(LED3, HIGH);
  LFR(105);
  digitalWrite(LED3, LOW);
}

void calibLine() {
  displayText(25, 35, "Line calibration", true);
  calibrateLine();
  displayText(10, 10, "min:", true);
  displayArray(lineCalibMin, 4, 10, 20);
  displayText(70, 10, "max:");
  displayArray(lineCalibMax, 4, 70, 20);
  while (digitalRead(BTN_SET));
  delay(250);
}

void displaySens() {
  while (digitalRead(BTN_PLUS)) {
    readSensors();
    u8g2.clearBuffer();
    displayArray(lineVals, 4, 10, 10);
  }
}

void displayCam() {
  while (digitalRead(BTN_PLUS)) {
    u8g2.clearBuffer();
    char c = getDirection();
    displayText(50, 50, &c);
  }
}

const char *string_list = 
  "Line calib\n"
  "Read sens\n"
  "Cam\n"
  "Run";

void loop() {
  static uint8_t curr;
  curr = u8g2.userInterfaceSelectionList(
    "Menu",
    curr, 
    string_list
  );
  if (curr == 1) calibLine();
  else if (curr == 2) displaySens();
  else if (curr == 3) displayCam();
  else if (curr == 4) run();
}
