#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN 7        // Capacitive sensor output pin
#define BUZZER_PIN 8        // Buzzer pin

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set the LCD address and size

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  
  pinMode(SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.setCursor(0, 1);
  lcd.print("Waiting for object");
}

void loop() {
  int sensorValue = digitalRead(SENSOR_PIN);

  if (sensorValue == HIGH) { // Object detected
    Serial.println("Plastic detected!");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Plastic detected!");
    digitalWrite(BUZZER_PIN, HIGH); // Sound buzzer
  } else { // No object detected
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("No object detected");
    digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
  }

  delay(100); // Small delay to stabilize readings
}
