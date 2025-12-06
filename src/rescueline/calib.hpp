#pragma once

#include <EEPROM.h>

extern int lineCalibMax[4], lineCalibMin[4];
extern uint16_t tcsCalibMinR[3], tcsCalibMaxR[3];
extern uint16_t tcsCalibMinL[3], tcsCalibMaxL[3];

void calibrateLine();
void calibrateTcs();

template<typename T>
static void getCalibEeprom(int adr, T* mx, T* mn, int size) {
  for (int i = 0; i < size; i++) {
    EEPROM.get(adr, mx[i]);
    adr += sizeof(T);
  }
  for (int i = 0; i < size; i++) {
    EEPROM.get(adr, mn[i]);
    adr += sizeof(T);
  }
}
