#pragma once

extern int lineVals[];

void readSensors();
void printSensors();
int readLine();
void LFR(int speed = 100);
int getNorm(int i);