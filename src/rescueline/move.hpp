#pragma once

void setMotor(int speed, int pin1, int pin2, int pwmPin);
void driveFront(int leftSpeed, int rightSpeed);
void driveBack(int leftSpeed, int rightSpeed);
void turnRightToBlack(int speed, int t);
void turnLeftToBlack(int speed, int t);
