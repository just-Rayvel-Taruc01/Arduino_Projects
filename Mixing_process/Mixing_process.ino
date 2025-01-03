#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <Adafruit_PWMServoDriver.h>
#include "ServoControl.h"

// Servo channels
#define SERVO1_CHANNEL 0  // Output door
#define SERVO2_CHANNEL 1  // Loading door
#define SERVO3_CHANNEL 2  // Unloading door

// Pin definitions
const int buttonPin = 10;
const int ssr2Pin = 9;
const int moistureSensorPin = A0;
const int lcdAddress = 0x27;

// Variables
int moistureLevel;
int moisturePercent;
unsigned long startTime = 0;
bool timerRunning = false;
bool mixingComplete = false;
unsigned long mixingTime = 180;    // in seconds
unsigned long backgroundTime = 30; // Additional 30 seconds to run in the background
const float calibration_factor = 450500.00;
const float targetWeightKg = 2.0;
HX711 scale;

// Button debounce
bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 10;  // Reduced to 10ms

// LCD Initialization
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting System");

  setupServos();  // Initialize servos to closed

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ssr2Pin, OUTPUT);
  digitalWrite(ssr2Pin, HIGH);  // Mixing OFF initially

  scale.begin(11, 12);  // HX711 Pins
  scale.set_scale(calibration_factor);
  scale.tare();

  lcd.init();
  lcd.backlight();
  displayInitialScreen();
}

void loop() {
  float weight = scale.get_units(5);
  lcd.setCursor(0, 1);
  lcd.print("Wt: ");
  lcd.print(weight, 2);
  lcd.print(" kg  ");  // Space-padding to prevent flicker

  if (weight >= targetWeightKg && !timerRunning) {
    lcd.setCursor(0, 0);
    lcd.print("Press Button   ");
  }

  handleButtonPress();

  if (timerRunning) {
    runProcess(millis());
  }
}

void handleButtonPress() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        timerRunning ? stopProcess() : startProcess();
      }
    }
  }
  lastButtonState = reading;
}

void startProcess() {
  startTime = millis();
  timerRunning = true;
  mixingComplete = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer Started ");
}

void runProcess(unsigned long currentMillis) {
  unsigned long processTime = (currentMillis - startTime) / 1000;

  moistureLevel = analogRead(moistureSensorPin);
  moisturePercent = constrain(map(moistureLevel, 595, 239, 0, 100), 0, 100);

  lcd.setCursor(0, 1);
  lcd.print("Moist: ");
  lcd.print(moisturePercent);
  lcd.print("%      ");

  if (!mixingComplete) {
    if (processTime < mixingTime) {
      digitalWrite(ssr2Pin, LOW);  // Keep mixing ON

      // During mixing, check moisture content periodically
      if (processTime % 20 == 0) {  // Check every 20 seconds
        if (moisturePercent > 70) {
          // Open loading door (servo 2) to add CRH
          setServoAngle(SERVO2_CHANNEL, OPEN_ANGLE);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Loading CRH... ");
          delay(3000);  // Let CRH load for 3 seconds
          setServoAngle(SERVO2_CHANNEL, CLOSED_ANGLE);

          // Open unloading door (servo 3) to drop CRH into the mixer
          setServoAngle(SERVO3_CHANNEL, OPEN_ANGLE);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Unloading CRH...");
          delay(3000);  // Let CRH drop for 3 seconds
          setServoAngle(SERVO3_CHANNEL, CLOSED_ANGLE);
        }
      }

      lcd.setCursor(0, 0);
      lcd.print("Mixing T: ");
      lcd.print(mixingTime - processTime);
      lcd.print("s ");
    } else {
      mixingComplete = true;
      digitalWrite(ssr2Pin, HIGH); // Mixing OFF
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mixing Done");
      lcd.setCursor(0, 1);
      lcd.print("Output Door Open");
      setServoAngle(SERVO1_CHANNEL, OPEN_ANGLE);  // Open output door to let material exit
      delay(backgroundTime * 1000);  // Wait for 30 seconds to let material exit
      setServoAngle(SERVO1_CHANNEL, CLOSED_ANGLE); // Close output door
      stopProcess();
    }
  }
}

void stopProcess() {
  timerRunning = false;
  digitalWrite(ssr2Pin, HIGH); // Mixing OFF
  setServoAngle(SERVO1_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO2_CHANNEL, CLOSED_ANGLE);
  setServoAngle(SERVO3_CHANNEL, CLOSED_ANGLE);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process Stopped");
  delay(500);
  displayInitialScreen();
}

void displayInitialScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Input 2 kg   ");
  lcd.setCursor(0, 1);
  lcd.print("Wt: ");
  lcd.print(scale.get_units(5), 2);
  lcd.print(" kg  ");
}
