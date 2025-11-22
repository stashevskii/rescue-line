#include <Arduino.h>
#include "lfr.hpp"
#include "move.hpp"

const int N = 4;
static constexpr double KP = 0.08;
static constexpr double KD = 0.8;
static constexpr int w[N] = {1000, 2000, 3000, 4000};
static constexpr int linePins[N] = {A14, A8, A6, A4};
static int lastPos = 2500;
int lineValues[N];

void readSensors() {
  for (int i = 0; i < N; ++i)
    lineValues[i] = analogRead(linePins[i]);
  for (int i = 0; i < N; ++i)
    lineValues[i] = (lineValues[i] + analogRead(linePins[i])) / 2;
}

void printSensors() {
  readSensors();
  for (int i = 0; i < N; ++i) {
    Serial.print(i);
    Serial.print(" - ");
    Serial.print(lineValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

int readLine() {
  int sumAll = 0;
  long sumW = 0;
  bool flag = false;
  readSensors();
  for (int i = 0; i < N; ++i) {
    if (lineValues[i] > 150){
      sumAll += lineValues[i];
      sumW += lineValues[i] * (long)w[i];
      flag = true;
    }
  }
  if (!flag) {
    return (lastPos < 2500 ? 0 : 5000);
  }
  lastPos = sumW / sumAll;
  return lastPos;
}

void LFR(int speed = 100) {
  int lastError = 0;
  while(true){
    int err = 2500 - readLine();
    int delta = (err * KP) + (err - lastError) * KD;
    lastError = err;
    driveFront(speed - delta, speed + delta);
    driveBack(speed - delta, speed + delta);
  }
}
