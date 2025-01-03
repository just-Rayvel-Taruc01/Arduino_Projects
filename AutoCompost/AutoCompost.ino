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
const int ssr1Pin = 8;
const int ssr2Pin = 9;
const int moistureSensorPin = A0;
const int lcdAddress = 0x27;

// Variables
int moistureLevel;
int moisturePercent;
unsigned long startTime = 0;
bool timerRunning = false;
bool shreddingComplete = false;
unsigned long shreddingTime = 30;  // in seconds
unsigned long mixingTime = 120;    // in seconds

const float calibration_factor = -450200.00;
const float targetWeightKg = 3.0;
HX711 scale;

// Button debounce
bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30;  // Reduced to 30ms

// LCD Initialization
LiquidCrystal_I2C lcd(lcdAddress, 20, 4);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting System");
  
  setupServos();  // Initialize servos to closed
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ssr1Pin, OUTPUT);
  pinMode(ssr2Pin, OUTPUT);
  
  digitalWrite(ssr1Pin, HIGH);  // OFF initially
  digitalWrite(ssr2Pin, HIGH);  // OFF initially
  
  scale.begin(11, 12);  // HX711 Pins
  scale.set_scale(calibration_factor);
  scale.tare();
  
  lcd.init();
  lcd.backlight();
  displayInitialScreen();
}

void loop() {
  float weight = scale.get_units(10);
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
  shreddingComplete = false;
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

  if (!shreddingComplete) {
    if (processTime < shreddingTime) {
      digitalWrite(ssr1Pin, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Shredding T: ");
      lcd.print(shreddingTime - processTime);
      lcd.print("s ");
    } else {
      digitalWrite(ssr1Pin, HIGH);
      shreddingComplete = true;
      startTime = currentMillis;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Shredding Done");
    }
  } else {
    if (processTime < mixingTime) {
      digitalWrite(ssr2Pin, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Mixing T: ");
      lcd.print(mixingTime - processTime);
      lcd.print("s ");
    } else {
      digitalWrite(ssr2Pin, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mixing Done");
      stopProcess();
    }
  }
}

void stopProcess() {
  timerRunning = false;
  digitalWrite(ssr1Pin, HIGH); // Shredding OFF
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
  lcd.print("Input 3 kg   ");
  lcd.setCursor(0, 1);
  lcd.print("Wt: ");
  lcd.print(scale.get_units(5), 2);
  lcd.print(" kg  ");
}
