#pragma once

// Motor Right
constexpr int MOTOR_LEFT_A_IN1 = 29;
constexpr int MOTOR_LEFT_A_IN2 = 31;
constexpr int MOTOR_LEFT_A_PWM = 44;

constexpr int MOTOR_RIGHT_A_IN1 = 25;
constexpr int MOTOR_RIGHT_A_IN2 = 27;
constexpr int MOTOR_RIGHT_A_PWM = 46;

// Motor Left
constexpr int MOTOR_RIGHT_B_IN1 = 9;
constexpr int MOTOR_RIGHT_B_IN2 = 11;
constexpr int MOTOR_RIGHT_B_PWM = 6;

constexpr int MOTOR_LEFT_B_IN1 = 13;
constexpr int MOTOR_LEFT_B_IN2 = 5;
constexpr int MOTOR_LEFT_B_PWM = 7;


constexpr int MOTOR_PINS[] = {
  // Motor Right A
  MOTOR_RIGHT_A_IN1,
  MOTOR_RIGHT_A_IN2,
  MOTOR_RIGHT_A_PWM,
  
  // Motor Right B
  MOTOR_RIGHT_B_IN1,
  MOTOR_RIGHT_B_IN2,
  MOTOR_RIGHT_B_PWM,

  // Motor Left A
  MOTOR_LEFT_A_IN1,
  MOTOR_LEFT_A_IN2,
  MOTOR_LEFT_A_PWM,

  // Motor Left B
  MOTOR_LEFT_B_IN1,
  MOTOR_LEFT_B_IN2,
  MOTOR_LEFT_B_PWM 
};

constexpr size_t MOTOR_PINS_COUNT = sizeof(MOTOR_PINS) / sizeof(MOTOR_PINS[0]);

// LEDs
constexpr int LED1 = 34;
constexpr int LED2 = 36;
constexpr int LED3 = 38;

// Buttons
constexpr int BTN_SET = 12;
constexpr int BTN_PLUS = 15;
constexpr int BTN_MINUS = 14;