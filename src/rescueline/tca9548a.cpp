#include <Wire.h>
#include "tca9548a.hpp"

static constexpr int ADR = 0x70;

void selectChannel(uint8_t bus) {
  if (bus > 7) return;
  Wire.beginTransmission(ADR);
  Wire.write(1 << bus);
  Wire.endTransmission();
}