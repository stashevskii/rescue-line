#include "config.hpp"
#include <EEPROM.h>

constexpr int BAUD_RATE = 19200;
constexpr int CHAR_BUF = 128;
constexpr double KP = 0.08;
constexpr double KD = 0.8;
constexpr int BASIC_SPEED = 100;
constexpr int N = 4;
constexpr int w[N] = {1000, 2000, 3000, 4000};
constexpr int linePins[N] = {A14, A8, A6, A4};
int lineValues[N], maxS[N], minS[N];
double err;
int blobPos;
int isCross;
int lastPos = 2500;

void setMotor(int speed, int pin1, int pin2, int pwmPin) {
  speed = constrain(speed, -100, 200);
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

void driveFront(int leftSpeed, int rightSpeed) {
  setMotor(rightSpeed, MOTOR_RIGHT_A_IN1, MOTOR_RIGHT_A_IN2, MOTOR_RIGHT_A_PWM);
  setMotor(leftSpeed, MOTOR_LEFT_A_IN1, MOTOR_LEFT_A_IN2, MOTOR_LEFT_A_PWM);
}

void driveBack(int leftSpeed, int rightSpeed) {
  setMotor(rightSpeed, MOTOR_RIGHT_B_IN1, MOTOR_RIGHT_B_IN2, MOTOR_RIGHT_B_PWM);
  setMotor(leftSpeed, MOTOR_LEFT_B_IN1, MOTOR_LEFT_B_IN2, MOTOR_LEFT_B_PWM);
}

void readSensors() {
  for (int i = 0; i < N; ++i) {
    lineValues[i] = analogRead(linePins[i]);
  }
  for (int i = 0; i < N; ++i) {
    lineValues[i] = (lineValues[i] + analogRead(linePins[i]))/2;
  }
}

void calibrate() {
  int maxS[N], minS[N];
  unsigned long tmr = millis();
  while (millis() <= tmr + 5000) {
    readSensors();
    for (int i = 0; i < N; ++i)
      maxS[i] = max(maxS[i], lineValues[i]);
    for (int i = 0; i < N; ++i)
      minS[i] = max(minS[i], lineValues[i]);
  }
  int adr = 0;
  EEPROM.put(adr, maxS);
  adr += sizeof(maxS);
  EEPROM.put(adr, minS);
}

void readSensorsCalib() {
  int a[N], b[N];
  int adr = 0;
  EEPROM.put(adr, a);
  adr += sizeof(a);
  EEPROM.put(adr, b);
  for (int i = 0; i < N; ++i) {
    lineValues[i] = analogRead(linePins[i]);
  }
  for (int i = 0; i < N; ++i) {
    lineValues[i] = (lineValues[i] + analogRead(linePins[i]))/2;
  }
  for (int i = 0; i < N; ++i) {
    lineValues[i] = map(lineValues[i], 0, 1000, a[i], b[i]);
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

int readLine() {
  int s = 0;
  long s2 = 0;
  bool onLine = false;
  readSensors();
  for (int i = 0; i < N; ++i) {
    if(lineValues[i] > 150){
      s += lineValues[i];
      s2 += lineValues[i] * (long)w[i];
      onLine = true;
    }
  }
  if(!onLine){
    if(lastPos<2500){
      return 0;
    }else{
      return 5000;
    }
  }
  lastPos = s2 / s;
  return lastPos;
}

void LFR(){
  int lastError = 0;
  while(true){
    int err = 2500 - readLine();
    int delta =(err*KP) + (err - lastError) * KD;
    lastError = err;
    // printSensors();
    Serial.println(delta);
    driveFront(BASIC_SPEED - delta, BASIC_SPEED + delta);
    driveBack(BASIC_SPEED - delta, BASIC_SPEED + delta);
  }
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

void testDrive(){
  while(true){
    driveFront(0, 100);
    driveBack(0, 100);
  }
}

void loop() {
  LFR();
  // testDrive();
}