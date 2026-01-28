#include <Arduino.h>
#include <Octoliner.h>
#include "config.hpp"
#include "move.hpp"
#include "lfr.hpp"
#include "calib.hpp"
#include "cam.hpp"

static constexpr double KP2 = 1;
static constexpr double KP = 0.17;
static constexpr double KD = 0.65;
static constexpr int THRESHOLD = 0;
static constexpr int w[6] = {1000, 2000, 3000, 4000, 5000, 6000};
static constexpr int linePins[6] = {A4, A6, A8, A10, A12, A14};
static constexpr int TARGET = 3500;
int lineVals[6];

int getNorm(int i) {
  return map(
    constrain(lineVals[i], lineCalibMin[i], lineCalibMax[i]),
    lineCalibMin[i],
    lineCalibMax[i],
    0, 1000
  );
}

void readSensors() {
  for (int i = 0; i < 6; i++) {
    lineVals[i] = (analogRead(linePins[i]) + analogRead(linePins[i])) / 2;
  }
}

int readLine() {
  static int lastPos = TARGET;
  int total = 0;
  long wsum = 0;
  bool flag = false;
  readSensors();
  for (int i = 0; i < 6; ++i) {
    int normolized = lineVals[i];
    if (normolized > THRESHOLD) {
      total += normolized;
      wsum += normolized * (long)w[i];
      flag = true;
    }
  }
  if (!flag) {
    return (lastPos < TARGET ? 0 : 7000);
  }
  lastPos = wsum / total;
  return lastPos;
}

void LFR(int speed = 100) {
  static int lastError = 0;
  while (true) {
    int err = TARGET - readLine();
    int delta = (err * KP) + (err - lastError) * KD;
    lastError = err;
    driveFront(speed - delta, speed + delta);
    driveBack(speed - delta, speed + delta);
  }
}

void CamLFR(int speed = 100) {
  while(true){
  String txt = Serial2.readStringUntil('\n');
  int deflAng = txt.toInt();
  Serial.println(deflAng);
  int delta = deflAng*KP2;
  driveFront(speed - delta, speed + delta);
  driveBack(speed - delta, speed + delta);
  }
}
