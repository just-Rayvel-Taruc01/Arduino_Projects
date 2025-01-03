#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal_I2C.h>

// Initialize Servo Shield at address 0x40
Adafruit_PWMServoDriver servoShield = Adafruit_PWMServoDriver(0x40);

// Initialize LCD at address 0x70 (replace this with the correct address if different)
LiquidCrystal_I2C lcd(0x70, 20, 4);  

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize Servo Shield
  servoShield.begin();
  servoShield.setPWMFreq(50);  // Set frequency for servo control (50Hz standard)

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Servo & LCD Test");
}

void loop() {
  // Move a servo connected to channel 0 from 0 to 180 degrees
  for (int angle = 150; angle < 600; angle += 10) {
    servoShield.setPWM(0, 0, angle);
    delay(100);
  }

  for (int angle = 600; angle > 150; angle -= 10) {
    servoShield.setPWM(0, 0, angle);
    delay(100);
  }

  // Update LCD display
  lcd.setCursor(0, 1);
  lcd.print("Servo Moving...  ");
}
