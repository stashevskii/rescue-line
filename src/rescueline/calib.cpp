#include <Arduino.h>
#include <EEPROM.h>
#include "move.hpp"
#include "calib.hpp"
#include "config.hpp"
#include "lfr.hpp"

void calibrate() {
  digitalWrite(LED1, HIGH);
  int maxS[N], minS[N];
  for (int i = 0; i < N; ++i) {
    minS[i] = 1000;
    maxS[i] = 0;
  }
  unsigned long tmr = millis();
  unsigned long tmr2 = millis();
  bool flag = true;
  while (millis() <= tmr + 10000) {
    while(millis() <= tmr2 + 800){
      readSensors();
      for (int i = 0; i < N; ++i) {
        maxS[i] = max(maxS[i], lineValues[i]);
        minS[i] = min(minS[i], lineValues[i]);
      }
      if(flag){
        driveFront(80, 80);
        driveBack(80, 80);
      }else{
        driveFront(-80, -80);
        driveBack(-80, -80);
      }
    }
    flag = !flag;
    tmr2 = millis();
  }
  int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, maxS[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.put(adr, minS[i]);
    adr += sizeof(int);
  }
  driveFront(0, 0);
  driveBack(0, 0);
  digitalWrite(LED1, LOW);
}

void fetchCalib(int* mx, int* mn) {
  digitalWrite(LED2, HIGH);
  int adr = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.get(adr, mx[i]);
    adr += sizeof(int);
  }
  for (int i = 0; i < N; i++) {
    EEPROM.get(adr, mn[i]);
    adr += sizeof(int);
  }
  digitalWrite(LED2, LOW);
}
