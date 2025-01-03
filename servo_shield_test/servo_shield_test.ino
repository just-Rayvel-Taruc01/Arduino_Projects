#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create the PWM servo driver object using the default I2C address (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo channels on the Adafruit Servo Shield (0-15)
#define SERVO1_CHANNEL 0
#define SERVO2_CHANNEL 1
#define SERVO3_CHANNEL 2

// Define the pulse lengths for open and closed positions
#define MIN_PULSE 150  // Pulse length corresponding to 0 degrees
#define MAX_PULSE 600  // Pulse length corresponding to 180 degrees
#define CLOSED_ANGLE  30
#define OPEN_ANGLE    90

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit Servo Shield - 3 Servo Door Control");

  // Initialize the PWM driver
  pwm.begin();
  pwm.setPWMFreq(60);  // Servos run at ~60 Hz

  // Move all servos to the closed position (30 degrees)
  setServoAngle(SERVO1_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO2_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO3_CHANNEL, CLOSED_ANGLE);
  delay(1000);  // Wait for 1 second (doors closed)

  // Move all servos to the open position (90 degrees)
  setServoAngle(SERVO1_CHANNEL, OPEN_ANGLE);
  setServoAngle(SERVO2_CHANNEL, OPEN_ANGLE);
  setServoAngle(SERVO3_CHANNEL, OPEN_ANGLE);
  delay(1000);  // Wait for 1 second (doors open)
}

void loop() {
  // Close all doors
  setServoAngle(SERVO1_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO2_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO3_CHANNEL, CLOSED_ANGLE);
  delay(2000);  // Wait 2 seconds with doors closed
  
  // Open all doors
  setServoAngle(SERVO1_CHANNEL, OPEN_ANGLE);
  setServoAngle(SERVO2_CHANNEL, OPEN_ANGLE);
  setServoAngle(SERVO3_CHANNEL, OPEN_ANGLE);
  delay(2000);  // Wait 2 seconds with doors open
}

// Function to set servo angle using the Adafruit Servo Shield
void setServoAngle(int servoChannel, int angle) {
  // Map the angle (0-180) to the pulse length (150-600)
  int pulseLength = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
  pwm.setPWM(servoChannel, 0, pulseLength);
}
