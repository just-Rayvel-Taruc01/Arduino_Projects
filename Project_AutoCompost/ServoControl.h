#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Adafruit_PWMServoDriver.h>

// Servo channels on the Adafruit Servo Shield (0-15)
#define SERVO1_CHANNEL 0  // Output door (for unloading)
#define SERVO2_CHANNEL 1  // Loading door
#define SERVO3_CHANNEL 2  // Unloading door

// Define the pulse lengths for open and closed positions
#define MIN_PULSE 150  // Pulse length corresponding to 0 degrees
#define MAX_PULSE 600  // Pulse length corresponding to 180 degrees
#define CLOSED_ANGLE 90
#define OPEN_ANGLE 0

extern Adafruit_PWMServoDriver pwm;

void setupServos();
void setServoAngle(int servoChannel, int angle);

#endif
