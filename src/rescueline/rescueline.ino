#include "config.hpp"
#include <Wire.h>

constexpr int BAUD_RATE = 19200;
constexpr int CHAR_BUF = 128;
constexpr int KP = 0.8;
constexpr int BASIC_SPEED = 70;

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
  if (Serial2.available()) {
    String buff = Serial2.readStringUntil('\n');
    int err = buff.toInt();
    driveFront(BASIC_SPEED - err, BASIC_SPEED + err);
    driveBack(BASIC_SPEED - err, BASIC_SPEED + err);
  }
}