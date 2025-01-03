#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoControl.h"

// Create the PWM servo driver object using the default I2C address (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setupServos() {
  pwm.begin();
  pwm.setPWMFreq(60);  // Servos run at ~60 Hz

  // Move all servos to the closed position (30 degrees)
  setServoAngle(SERVO1_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO2_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO3_CHANNEL, CLOSED_ANGLE);
  delay(1000);  // Wait for 1 second (doors closed)
}

void setServoAngle(int servoChannel, int angle) {
  int pulseLength = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
  pwm.setPWM(servoChannel, 0, pulseLength);
}
