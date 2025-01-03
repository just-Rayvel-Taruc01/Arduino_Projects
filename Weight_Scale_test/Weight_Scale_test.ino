#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

// Pin definitions
const int hx711_dout = 11;
const int hx711_sck = 12;
const int lcdAddress = 0x27;

HX711 scale;
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

// Calibration and thresholds
const float calibration_factor = 648500.00;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  scale.begin(hx711_dout, hx711_sck);
  scale.set_scale(calibration_factor);
  scale.tare();
  lcd.setCursor(0, 1);
  lcd.print("Wt: 0.00 kg");
}

void loop() {
  float weight = scale.get_units(5);
  lcd.setCursor(0, 1);
  lcd.print("Wt: ");
  lcd.print(weight, 2);
  lcd.print(" kg    ");
}
