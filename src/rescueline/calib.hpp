#pragma once

#include <EEPROM.h>

extern int lineCalibMax[6], lineCalibMin[6];

void calibrateLine();
void calibrateTcs();

template<typename T>
inline void getCalibEeprom(int adr, T* mx, T* mn, int size) {
  for (int i = 0; i < size; i++) {
    EEPROM.get(adr, mx[i]);
    adr += sizeof(T);
  }
  for (int i = 0; i < size; i++) {
    EEPROM.get(adr, mn[i]);
    adr += sizeof(T);
  }
}
