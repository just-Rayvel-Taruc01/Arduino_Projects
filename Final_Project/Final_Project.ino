#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

// Define pin connections
const int buttonPin = 10;         // Arcade button pin
const int ssr1Pin = 8;            // SSR 1 control (Shredding)
const int ssr2Pin = 9;            // SSR 2 control (Mixing)
const int moistureSensorPin = A0; // Capacitive moisture sensor analog pin
const int lcdAddress = 0x27;      // I2C address of the LCD

const int dry = 595;              // Dry state sensor value
const int wet = 239;              // Wet state sensor value

// Initialize the LCD
LiquidCrystal_I2C lcd(lcdAddress, 16, 4);

bool buttonPressed = false;
bool timerRunning = false;
bool shreddingComplete = false;
unsigned long startTime = 0;
unsigned long previousMillis = 0;

// Variables for moisture sensor
int moistureLevel;
int moisturePercent;

// Variables for process timing
unsigned long shreddingTime = 30;  // 30 seconds for shredding
unsigned long mixingTime = 120;    // 120 seconds for mixing
unsigned long processTime;         // Tracks the current process time

// Calibration for weight
const float calibration_factor = 648500.00;
const float targetWeightKg = 3.0;  // Threshold weight in kg
HX711 scale;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(ssr1Pin, OUTPUT);        
  pinMode(ssr2Pin, OUTPUT);        

  // Initialize SSRs to OFF (assuming HIGH deactivates SSR)
  digitalWrite(ssr1Pin, HIGH); // Shredding OFF initially
  digitalWrite(ssr2Pin, HIGH); // Mixing OFF initially

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize the weight scale
  scale.begin(11, 12); // Pin 11 and 12 for HX711
  scale.set_scale(calibration_factor);
  scale.tare(); // Reset the scale to zero

  // Display initial message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Input 3 kg     ");

  // Display weight
  lcd.setCursor(0, 1);
  lcd.print("Wt: 0.00 kg    ");
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle button press with debounce logic
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;

  // Check if the weight is above the target
  float weight = scale.get_units(5); // Read weight with average of 5 readings

  lcd.setCursor(0, 1);
  lcd.print("Wt: ");
  lcd.print(weight, 2);
  lcd.print(" kg  ");

  // If weight is sufficient, show button message
  if (weight >= targetWeightKg) {
    lcd.setCursor(0, 0);
    lcd.print("Press Button");
  } else {
    // Only show "Input 3 kg of greens" if weight is below threshold
    lcd.setCursor(0, 0);
    lcd.print("Input 3 kg     ");
  }

  if (digitalRead(buttonPin) == LOW && (currentMillis - lastDebounceTime) > debounceDelay && !buttonPressed) {
    lastDebounceTime = currentMillis;
    buttonPressed = true;

    if (!timerRunning) {
      startTime = currentMillis;
      timerRunning = true;
      shreddingComplete = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Timer Started");
    } else {
      stopProcess();
    }
  }

  if (timerRunning) {
    runProcess(currentMillis);
  }

  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = false;
  }
}

void stopProcess() {
  timerRunning = false;
  digitalWrite(ssr1Pin, HIGH); // Shredding OFF
  digitalWrite(ssr2Pin, HIGH); // Mixing OFF
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process Stopped");
  delay(500);
}

void runProcess(unsigned long currentMillis) {
  processTime = (currentMillis - startTime) / 1000; // Time in seconds

  moistureLevel = analogRead(moistureSensorPin);
  moisturePercent = map(moistureLevel, dry, wet, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  lcd.setCursor(0, 1);
  lcd.print("Moist: ");
  lcd.print(moisturePercent);
  lcd.print("%      ");

  if (!shreddingComplete) {
    if (processTime < shreddingTime) {
      // Shredding in progress
      digitalWrite(ssr1Pin, LOW); // Turn on shredding
      lcd.setCursor(0, 0);
      lcd.print("Shredding T: ");
      lcd.print(shreddingTime - processTime); // Remaining shredding time
      lcd.print("s ");
    } else {
      // Shredding complete, move to mixing
      digitalWrite(ssr1Pin, HIGH); // Turn off shredding
      shreddingComplete = true;
      startTime = currentMillis; // Reset start time for mixing
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Shredding Done");
      delay(500); // Show done message
    }
  } else {
    // Mixing in progress
    if (processTime < mixingTime) {
      digitalWrite(ssr2Pin, LOW); // Turn on mixing
      lcd.setCursor(0, 0);
      lcd.print("Mixing T: ");
      lcd.print(mixingTime - processTime); // Remaining mixing time
      lcd.print("s ");
    } else {
      // Mixing complete
      digitalWrite(ssr2Pin, HIGH); // Turn off mixing
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mixing Done");
      delay(500); // Show done message
      stopProcess();
    }
  }
}
