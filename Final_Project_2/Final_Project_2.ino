#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

// Pin definitions
const int hx711_dout = 11;
const int hx711_sck = 12;
const int buttonPin = 10;
const int relay1Pin = 8;
const int relay2Pin = 9;
const int lcdAddress = 0x27;

HX711 scale;
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

// Calibration and thresholds
const float calibration_factor = 648500.00;
const float targetWeightKg = 3.0;

String message = "Please Input 3KG of Greens   ";
String startMessage = "Please press button to start   ";
unsigned long lastScrollTime = 0, lastWeightTime = 0, processStartTime = 0;
int scrollPosition = 0;
bool processRunning = false, buttonPressed = false;
int currentPhase = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);

  scale.begin(hx711_dout, hx711_sck);
  scale.set_scale(calibration_factor);
  scale.tare();
  lcd.setCursor(0, 1);
  lcd.print("Wt: 0.00 kg");
}

void loop() {
  unsigned long currentMillis = millis();

  if (!processRunning) {
    if (currentMillis - lastScrollTime >= 300) {
      lastScrollTime = currentMillis;
      lcd.setCursor(0, 0);
      lcd.print(message.substring(scrollPosition, scrollPosition + 16));
      scrollPosition = (scrollPosition + 1) % (message.length() - 15);
    }

    float weight = scale.get_units(5);
    lcd.setCursor(0, 1);
    lcd.print("Wt: ");
    lcd.print(weight, 2);
    lcd.print(" kg    ");

    if (weight >= targetWeightKg) {
      lcd.setCursor(0, 0);
      lcd.print(startMessage.substring(scrollPosition, scrollPosition + 16));
      if (digitalRead(buttonPin) == LOW) {
        buttonPressed = true;
        startProcess();
      }
    }
  } else {
    runProcess(currentMillis);
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true;
      stopProcess();
    }
  }
  
  if (digitalRead(buttonPin) == HIGH) buttonPressed = false;
}

void startProcess() {
  processRunning = true;
  processStartTime = millis();
  currentPhase = 1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shredding");
  digitalWrite(relay1Pin, LOW);
}

void runProcess(unsigned long currentMillis) {
  unsigned long elapsedTime = (currentMillis - processStartTime) / 1000;

  switch (currentPhase) {
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(30 - elapsedTime);
      lcd.print(" sec   ");
      if (elapsedTime >= 30) {
        currentPhase = 2;
        processStartTime = millis();
        digitalWrite(relay1Pin, HIGH);
        digitalWrite(relay2Pin, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mixing");
      }
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(180 - elapsedTime);
      lcd.print(" sec  ");
      if (elapsedTime >= 180) {
        stopProcess();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Process Complete");
      }
      break;
  }
}

void stopProcess() {
  processRunning = false;
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process Stopped");
}
