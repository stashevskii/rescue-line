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

int getLine() {
  static int32_t temp = 0;
  static char buff[CHAR_BUF] = {0};

  Wire.requestFrom(0x12, 2);
  if (Wire.available() == 2) {

    temp = Wire.read() | (Wire.read() << 8);

    Wire.requestFrom(0x12, temp);
    if (Wire.available() == temp) {
      temp = 0;
      while(Wire.available()) buff[temp++] = Wire.read();

    } else {
      while(Wire.available()) Wire.read();
    }
  } else {
    while (Wire.available()) Wire.read();
  }
  return atoi(buff);
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
    int err = Serial2.read();
    Serial.println(err);
  }else{
    Serial.println("ne rabotaet");
  }
  //err *= 0.8;
  // driveFront(BASIC_SPEED - err, BASIC_SPEED + err);
  // driveBack(BASIC_SPEED - err, BASIC_SPEED + err);
}