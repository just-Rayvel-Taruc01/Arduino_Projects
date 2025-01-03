#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pin connections
const int moistureSensorPin = A0; // Capacitive moisture sensor analog pin
const int lcdAddress = 0x27;      // I2C address of the LCD

// Moisture sensor calibration values
const int dry = 595;  // Value for Dry State Sensor
const int wet = 239;  // Value for Wet State Sensor

// Initialize the LCD
LiquidCrystal_I2C lcd(lcdAddress, 20, 4);

void setup() {
  // Initialize LCD
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Moisture Sensor");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Read moisture sensor value
  int moistureLevel = analogRead(moistureSensorPin);
  
  // Convert the raw value to percentage (100% wet, 0% dry)
  int moisturePercent = map(moistureLevel, dry, wet, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100); // Ensure values stay within 0-100

  // Display the moisture percentage on LCD
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moisturePercent);
  lcd.print("%    "); // Add spaces to clear previous values

  delay(500); // Update every 500 ms
}
