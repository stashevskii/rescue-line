#pragma once

extern void readSensors();
extern int lineValues[];
extern const int N; 

void calibrate();
void readCalibValues(int* mx, int* mn);
