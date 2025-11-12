#include "config.hpp"
#include <Wire.h>

constexpr int BAUD_RATE = 19200;
constexpr int CHAR_BUF = 128;
constexpr double KP = 3;
constexpr int BASIC_SPEED = 45;
constexpr int N = 4;
constexpr int w[N] = {1000, 2000, 3000, 4000};
constexpr int linePins[N] = {A14, A6, A4, A8};
int lineValues[N];
double err;
int blobPos;
int isCross;

void setMotor(int speed, int pin1, int pin2, int pwmPin) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    analogWrite(pwmPin, abs(speed));
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    analogWrite(pwmPin, abs(speed));
  }
}

void driveFront(int rightSpeed, int leftSpeed) {
  setMotor(rightSpeed, MOTOR_RIGHT_A_IN1, MOTOR_RIGHT_A_IN2, MOTOR_RIGHT_A_PWM);
  setMotor(leftSpeed, MOTOR_LEFT_A_IN1, MOTOR_LEFT_A_IN2, MOTOR_LEFT_A_PWM);
}

void driveBack(int rightSpeed, int leftSpeed) {
  setMotor(rightSpeed, MOTOR_RIGHT_B_IN1, MOTOR_RIGHT_B_IN2, MOTOR_RIGHT_B_PWM);
  setMotor(leftSpeed, MOTOR_LEFT_B_IN1, MOTOR_LEFT_B_IN2, MOTOR_LEFT_B_PWM);
}

void readSensors() {
  for (int i = 0; i < N; ++i) {
    lineValues[i] = analogRead(linePins[i]);
  }
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

double getNormalized() {
  int s = 0, s2 = 0;
  readSensors();
  for (int i = 0; i < N; ++i) {
    s += lineValues[i];
    s2 += lineValues[i] * w[i];
  }
  return (double)s2 / s;
}

double getError() {
  return 2500.0 - getNormalized();
}

void standBy() {
  pinMode(30, OUTPUT);
  digitalWrite(30, HIGH);
}

void setup() {
  for (int i = 0; i < 12; i++) {
    pinMode(MOTOR_PINS[i], OUTPUT);
  }
  standBy();
  Serial.begin(BAUD_RATE);
  Serial2.begin(BAUD_RATE);
}

void loop() {
  printSensors();
  double err = getError();
  err *= KP;
  delay(400);
  /*driveFront(BASIC_SPEED + err, BASIC_SPEED - err);
  driveBack(BASIC_SPEED + err, BASIC_SPEED - err);*/
}