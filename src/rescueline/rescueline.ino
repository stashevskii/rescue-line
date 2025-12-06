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
  tcs.begin();
  getCalibEeprom(0, lineCalibMax, lineCalibMin, 4);
  getCalibEeprom(8 * sizeof(int), tcsCalibMaxR, tcsCalibMinR, 3);
  getCalibEeprom(14 * sizeof(int), tcsCalibMaxL, tcsCalibMinL, 3);
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

void calibTcs() {
  displayText(25, 35, "Calibration TCS34725", true);
  calibrateTcs();
  displayText(10, 10, "min1:", true);
  displayArray(tcsCalibMinR, 3, 20, 20);
  displayText(70, 10, "max1:");
  displayArray(tcsCalibMaxR, 3, 80, 20);
  while (digitalRead(BTN_SET));
  displayText(10, 10, "min2:", true);
  displayArray(tcsCalibMinL, 3, 20, 20);
  displayText(70, 10, "max2:");
  displayArray(tcsCalibMaxL, 3, 80, 20);
  while (digitalRead(BTN_SET));
  delay(250);
}

void displayTcs() {
  uint16_t a[3];
  uint8_t ch = RIGHT_TCS;
  while (digitalRead(BTN_SET)) {
    if (!digitalRead(BTN_PLUS)) {
      ch = ch == RIGHT_TCS ? LEFT_TCS : RIGHT_TCS;
      delay(30);
    }
    getRGBMap(&a[0], &a[1], &a[2], ch);
    u8g2.clearBuffer();
    displayArray(a, 3, 10, 10);
    displayText(40, 40, (ch == RIGHT_TCS ? "RIGHT" : "LEFT"));
  }
}

void displaySens() {
  while (digitalRead(BTN_PLUS)) {
    readSensors();
    u8g2.clearBuffer();
    displayArray(lineVals, 4, 10, 10);
  }
}

const char *string_list = 
  "Line calib\n"
  "CS calib\n"
  "Read CS\n"
  "Read sens\n"
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
  else if (curr == 4) displaySens();
  else if (curr == 5) run();
}
