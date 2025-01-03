#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoControl.h"

// Servo channels
#define SERVO1_CHANNEL 0  // Output door
#define SERVO2_CHANNEL 1  // Loading door
#define SERVO3_CHANNEL 2  // Unloading door

// Pin definitions
const int buttonPin = 10;
const int lcdAddress = 0x27;

// LCD Initialization
LiquidCrystal_I2C lcd(lcdAddress, 16, 4);

// Servo control angles
#define OPEN_ANGLE 30
#define CLOSED_ANGLE 90

void setup() {
  Serial.begin(9600);
  Serial.println("Servo Test Started");

  setupServos();  // Initialize servos to closed (CLOSED_ANGLE)

  pinMode(buttonPin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  displayInitialScreen();
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {
    testServos();
  }
}

void testServos() {
  // Open loading door (Servo 2)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Opening Loading ");
  lcd.setCursor(0, 1);
  lcd.print("Door (Servo 2) ");
  setServoAngle(SERVO2_CHANNEL, OPEN_ANGLE);
  delay(2000);  // Keep the loading door open for 2 seconds

  // Close loading door (Servo 2)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Closing Loading ");
  lcd.setCursor(0, 1);
  lcd.print("Door (Servo 2) ");
  setServoAngle(SERVO2_CHANNEL, CLOSED_ANGLE);
  delay(2000);  // Keep the loading door closed for 2 seconds

  // Open unloading door (Servo 3)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Opening Unload ");
  lcd.setCursor(0, 1);
  lcd.print("Door (Servo 3) ");
  setServoAngle(SERVO3_CHANNEL, OPEN_ANGLE);
  delay(2000);  // Keep the unloading door open for 2 seconds

  // Close unloading door (Servo 3)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Closing Unload ");
  lcd.setCursor(0, 1);
  lcd.print("Door (Servo 3) ");
  setServoAngle(SERVO3_CHANNEL, CLOSED_ANGLE);
  delay(2000);  // Keep the unloading door closed for 2 seconds

  // Test output door (Servo 1)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Opening Output ");
  lcd.setCursor(0, 1);
  lcd.print("Door (Servo 1) ");
  setServoAngle(SERVO1_CHANNEL, OPEN_ANGLE);
  delay(2000);  // Keep the output door open for 2 seconds

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Closing Output ");
  lcd.setCursor(0, 1);
  lcd.print("Door (Servo 1) ");
  setServoAngle(SERVO1_CHANNEL, CLOSED_ANGLE);
  delay(2000);  // Keep the output door closed for 2 seconds

  displayInitialScreen();
}

void displayInitialScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Servo Test Ready ");
  lcd.setCursor(0, 1);
  lcd.print("Press button to ");
  lcd.setCursor(0, 2);
  lcd.print("Test Servos");
}
