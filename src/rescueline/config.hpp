#pragma once

constexpr int BAUD_RATE = 9600;

constexpr int MOTOR_LEFT_A_IN1 = 29;
constexpr int MOTOR_LEFT_A_IN2 = 31;
constexpr int MOTOR_LEFT_A_PWM = 44;

constexpr int MOTOR_RIGHT_A_IN1 = 25;
constexpr int MOTOR_RIGHT_A_IN2 = 27;
constexpr int MOTOR_RIGHT_A_PWM = 46;

constexpr int MOTOR_RIGHT_B_IN1 = 9;
constexpr int MOTOR_RIGHT_B_IN2 = 11;
constexpr int MOTOR_RIGHT_B_PWM = 6;

constexpr int MOTOR_LEFT_B_IN1 = 13;
constexpr int MOTOR_LEFT_B_IN2 = 5;
constexpr int MOTOR_LEFT_B_PWM = 7;

constexpr int LED1 = 34;
constexpr int LED2 = 36;
constexpr int LED3 = 38;

constexpr int OUTPUT_MODE[] = {
  MOTOR_RIGHT_A_IN1,
  MOTOR_RIGHT_A_IN2,
  MOTOR_RIGHT_A_PWM,
  
  MOTOR_RIGHT_B_IN1,
  MOTOR_RIGHT_B_IN2,
  MOTOR_RIGHT_B_PWM,

  MOTOR_LEFT_A_IN1,
  MOTOR_LEFT_A_IN2,
  MOTOR_LEFT_A_PWM,

  MOTOR_LEFT_B_IN1,
  MOTOR_LEFT_B_IN2,
  MOTOR_LEFT_B_PWM,

  LED1,
  LED2,
  LED3,
  30
};

constexpr int BTN_SET = 12;
constexpr int BTN_PLUS = 15;
constexpr int BTN_MINUS = 14;

constexpr int BUTTONS[] = {BTN_SET, BTN_PLUS, BTN_MINUS};
