#pragma once

extern int lineVals[];

void readSensors();
int readLine();
void initOcto();
void LFR(int speed = 100);
void CamLFR(int speed = 100);
int getNorm(int i);