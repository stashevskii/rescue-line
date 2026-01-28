#include <Arduino.h>
#include "move.hpp"
#include "config.hpp"
#include "lfr.hpp"

void setMotor(int speed, int pin1, int pin2, int pwmPin) {
  speed = constrain(speed, -255, 255);
  bool cond = (speed >= 0);
  digitalWrite(pin1, cond);
  digitalWrite(pin2, !cond);
  analogWrite(pwmPin, abs(speed));
}

void driveFront(int leftSpeed, int rightSpeed) {
  setMotor(rightSpeed, MOTOR_RIGHT_A_IN1, MOTOR_RIGHT_A_IN2, MOTOR_RIGHT_A_PWM);
  setMotor(leftSpeed, MOTOR_LEFT_A_IN1, MOTOR_LEFT_A_IN2, MOTOR_LEFT_A_PWM);
}

void driveBack(int leftSpeed, int rightSpeed) {
  setMotor(rightSpeed, MOTOR_RIGHT_B_IN1, MOTOR_RIGHT_B_IN2, MOTOR_RIGHT_B_PWM);
  setMotor(leftSpeed, MOTOR_LEFT_B_IN1, MOTOR_LEFT_B_IN2, MOTOR_LEFT_B_PWM);
}

void turnRightToBlack(int speed, int t) {
  driveFront(speed, speed);
  driveBack(speed, -speed);
  delay(t);
  driveFront(speed, -speed);
  driveBack(speed, -speed);
  do {
    readSensors();
    Serial.println(getNorm(1));
  } while (getNorm(1) < 500);
  driveFront(0, 0);
  driveBack(0, 0);
}

void turnLeftToBlack(int speed, int t) {
  driveFront(-speed, speed);
  driveBack(-speed, speed);
  delay(t);
  driveFront(speed, -speed);
  driveBack(speed, -speed);
  do {
    readSensors();
    Serial.println(getNorm(1));
  } while (getNorm(2) < 500);
  driveFront(0, 0);
  driveBack(0, 0);
}