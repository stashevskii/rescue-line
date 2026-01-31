#include <Arduino.h>
#include "config.hpp"
#include "move.hpp"
#include "lfr.hpp"
#include "calib.hpp"
#include "cam.hpp"

static constexpr double KP2 = 2;
static constexpr double KP = 0.1;
static constexpr double KD = 0.9;
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
  while(true){
    int err = 2500 - readLine();
    int delta = (err * KP) + (err - lastError) * KD;
    lastError = err;
    driveFront(speed - delta, speed + delta);
    driveBack(speed - delta, speed + delta);
    readSensors();
    char resp = getDirection();
    if (resp == '1') {
      turnRightToBlack(120, 350);
    } else if (resp == '2') {
      turnLeftToBlack(120, 150);
    } else if (resp == '3') {
      turnRightToBlack(120, 150);
    }
  }
}

char buff[20];
uint8_t cur = 0;

void CamLFR(int speed = 100) {
  while (true) {
    while (Serial2.available()) {
      char c = Serial2.read();
      if (c == '\n') {
        buff[cur] = '\0';
        int err = -(atoi(buff));
        int delta = err * 3.7;
        memset(buff, '\0', sizeof(buff));
        cur = 0;
        driveFront(speed - delta, speed + delta);
        driveBack(speed - delta, speed + delta);
      } else {
        buff[cur++] = c;
      }
    }
  }
}
