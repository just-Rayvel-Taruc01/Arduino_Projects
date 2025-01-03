#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pin connections
const int buttonPin = 10;         // Arcade button pin
const int ssr1Pin = 8;            // SSR 1 control (Channel 1)
const int ssr2Pin = 9;            // SSR 2 control (Channel 2)
const int trigPin = 7;            // Ultrasonic sensor TRIG
const int echoPin = 8;            // Ultrasonic sensor ECHO
const int moistureSensorPin = A0; // Capacitive moisture sensor analog pin
const int lcdAddress = 0x27;      // I2C address of the LCD

const int dry = 595;              // Dry state sensor value
const int wet = 239;              // Wet state sensor value

// Initialize the LCD
LiquidCrystal_I2C lcd(lcdAddress, 16, 4);

bool buttonPressed = false;
bool timerRunning = false;
unsigned long startTime = 0;
unsigned long previousMillis = 0;

// Variables for Ultrasonic Sensor
long duration;
float distance;
const float containerHeight = 50.0; // Container height in inches
int percentageFilled;

// Variables for moisture sensor
int moistureLevel;
int moisturePercent;

// Variables for scrolling display
const char* scrollMessage = "Press Button to Start";
int scrollIndex = 0;
unsigned long scrollDelay = 300;
unsigned long lastScrollTime = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(ssr1Pin, OUTPUT);        
  pinMode(ssr2Pin, OUTPUT);        
  pinMode(trigPin, OUTPUT);        
  pinMode(echoPin, INPUT);         
  
  // Initialize SSRs to OFF (assuming HIGH deactivates SSR)
  digitalWrite(ssr1Pin, LOW);
  digitalWrite(ssr2Pin, LOW);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle button press with debounce logic
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;

  if (digitalRead(buttonPin) == LOW && (currentMillis - lastDebounceTime) > debounceDelay && !buttonPressed) {
    lastDebounceTime = currentMillis;
    buttonPressed = true;

    if (!timerRunning) {
      startTime = currentMillis;
      timerRunning = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Timer Started");
    } else {
      stopProcess();
    }
  }

  if (timerRunning) {
    runTimer(currentMillis);
  } else {
    displayScrollingText(currentMillis);
    monitorFillLevel(currentMillis);
  }

  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = false;
  }
}

void stopProcess() {
  timerRunning = false;
  digitalWrite(ssr1Pin, LOW);
  digitalWrite(ssr2Pin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process Stopped");
  delay(500);
}

void runTimer(unsigned long currentMillis) {
  unsigned long elapsedTime = (currentMillis - startTime) / 1000;

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(elapsedTime);
  lcd.print("s ");

  moistureLevel = analogRead(moistureSensorPin);
  moisturePercent = map(moistureLevel, dry, wet, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Moist: ");
  lcd.print(moisturePercent);
  lcd.print("%      ");

  if (elapsedTime < 40) {
    digitalWrite(ssr1Pin, HIGH);  // SSR 1 ON
  } else {
    digitalWrite(ssr1Pin, LOW); // SSR 1 OFF
  }

  if (elapsedTime >= 30) {
    digitalWrite(ssr2Pin, HIGH);  // SSR 2 ON
  } else {
    digitalWrite(ssr2Pin, LOW); // SSR 2 OFF
  }

  if (elapsedTime >= 60) {
    stopProcess();
  }
}

void monitorFillLevel(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 25000);
    distance = duration * 0.034 / 2 / 2.54;

    if (distance < containerHeight) {
      percentageFilled = round(((containerHeight - distance) / containerHeight) * 100);
    } else {
      percentageFilled = 0;
    }

    lcd.setCursor(0, 1);
    lcd.print("Filled: ");
    lcd.print(percentageFilled);
    lcd.print("% ");

    if (percentageFilled >= 100) {
      lcd.setCursor(0, 1);
      lcd.print("ALERT: FULL ");
    }
  }
}

void displayScrollingText(unsigned long currentMillis) {
  if (currentMillis - lastScrollTime >= scrollDelay) {
    lastScrollTime = currentMillis;
    lcd.clear();
    lcd.setCursor(0, 0);

    if (scrollIndex < strlen(scrollMessage)) {
      lcd.print(scrollMessage + scrollIndex);
    } else {
      lcd.print(scrollMessage);
    }
    scrollIndex++;
    if (scrollIndex > strlen(scrollMessage)) {
      scrollIndex = 0;
    }
  }
}
