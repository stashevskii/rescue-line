#pragma once

extern int lineCalibMax[4], lineCalibMin[4];
extern uint16_t tcsCalibMin[3], tcsCalibMax[3];

void calibrateLine();
void calibrateTcs();
