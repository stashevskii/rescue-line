#include <Arduino.h>
#include "config.hpp"
#include "move.hpp"
#include "lfr.hpp"
#include "calib.hpp"
#include "cam.hpp"

static constexpr double KP = 0.1;
static constexpr double KD = 1.1;
static constexpr int THRESHOLD = 250;
static constexpr int w[4] = {1000, 2000, 3000, 4000};
static constexpr int linePins[4] = {A14, A8, A6, A4};
int lineVals[4];

int getNorm(int i) {
  return map(constrain(lineVals[i], lineCalibMin[i], lineCalibMax[i]), lineCalibMin[i], lineCalibMax[i], 0, 1000);
}

void readSensors() {
  for (int i = 0; i < 4; ++i)
    lineVals[i] = analogRead(linePins[i]);
  for (int i = 0; i < 4; ++i)
    lineVals[i] = (lineVals[i] + analogRead(linePins[i])) / 2;
}

int readLine() {
  static int lastPos = 2500;
  int total = 0;
  long wsum = 0;
  bool flag = false;
  readSensors();
  for (int i = 0; i < 4; ++i) {
    int normolized = getNorm(i);
    if (normolized > THRESHOLD) {
      total += normolized;
      wsum += normolized * (long)w[i];
      flag = true;
    }
  }
  if (!flag) {
    return (lastPos < 2500 ? 0 : 5000);
  }
  lastPos = wsum / total;
  return lastPos;
}

void LFR(int speed = 100) {
  int lastError = 0;
  while (1) {
    int err = 2500 - readLine();
    int delta = (err * KP) + (err - lastError) * KD;
    lastError = err;
    driveFront(speed - delta, speed + delta);
    driveBack(speed - delta, speed + delta);
    readSensors();
    char resp = getDirection();
    if (resp == '1') {
      digitalWrite(LED1, 1);
      turnRightToBlack(140, 350);
      digitalWrite(LED1, 0);
    } else if (resp == '2') {
      digitalWrite(LED1, 1);
      turnLeftToBlack(140, 150);
      digitalWrite(LED1, 0);
    } else if (resp == '3') {
      digitalWrite(LED1, 1);
      turnRightToBlack(140, 150);
      digitalWrite(LED1, 0);
    }
  }
}
