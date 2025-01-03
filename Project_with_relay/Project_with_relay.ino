#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pin connections
const int buttonPin = 10;       // Pin connected to the arcade button
const int relay1Pin = 3;       // Pin connected to Relay 1 IN1
const int relay2Pin = 4;       // Pin connected to Relay 2 IN2
const int trigPin = 7;         // Pin connected to Ultrasonic sensor TRIG
const int echoPin = 8;         // Pin connected to Ultrasonic sensor ECHO
const int lcdAddress = 0x27;   // I2C address of the LCD

// Initialize the LCD
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

bool buttonPressed = false;    // State of button press
bool timerRunning = false;     // State of timer
unsigned long startTime = 0;   // Start time for the timer
unsigned long previousMillis = 0; // Time tracking for non-blocking delays

// Variables for Ultrasonic Sensor
long duration;
float distance;
const float containerHeight = 50.0; // 50 inches container height
int percentageFilled;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor
  pinMode(relay1Pin, OUTPUT);        // Set relay 1 pin as output
  pinMode(relay2Pin, OUTPUT);        // Set relay 2 pin as output
  pinMode(trigPin, OUTPUT);          // Set ultrasonic sensor TRIG as output
  pinMode(echoPin, INPUT);           // Set ultrasonic sensor ECHO as input

  // Initialize relays to off
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Press Button");
}

void loop() {
  unsigned long currentMillis = millis();

  // Debounce the button press (simple debounce)
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;  // 50ms debounce time
  
  if (digitalRead(buttonPin) == LOW && (currentMillis - lastDebounceTime) > debounceDelay && !buttonPressed) {
    lastDebounceTime = currentMillis;
    buttonPressed = true; // Register button press

    if (!timerRunning) {
      // Start the timer if it's not running
      startTime = currentMillis;   // Store the start time
      timerRunning = true;         // Start the timer
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Timer Started");
    } else {
      // Stop the timer if it's already running (second press)
      timerRunning = false;        // Stop the timer
      digitalWrite(relay1Pin, LOW); // Turn off Relay 1
      digitalWrite(relay2Pin, LOW); // Turn off Relay 2
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Process Stopped");
      delay(500);                  // Brief delay to debounce and display message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press Button");
    }
  }

  // Run the timer if it is active
  if (timerRunning) {
    unsigned long elapsedTime = (currentMillis - startTime) / 1000; // Elapsed time in seconds

    // Display elapsed time on LCD
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(elapsedTime);
    lcd.print(" s  ");  // Display in seconds

    // Relay 1 control: Active for the first 40 seconds
    if (elapsedTime < 40) {
      digitalWrite(relay1Pin, HIGH);  // Relay 1 ON
    } else {
      digitalWrite(relay1Pin, LOW);   // Relay 1 OFF
    }

    // Relay 2 control: Active starting at 15 seconds
    if (elapsedTime >= 15) {
      digitalWrite(relay2Pin, HIGH);   // Relay 2 ON
    } else {
      digitalWrite(relay2Pin, LOW);    // Ensure Relay 2 remains OFF before 15 seconds
    }

    // Stop the timer after 1 minute (60 seconds)
    if (elapsedTime >= 60) {
      timerRunning = false;  // Stop the timer
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Timer Ended");
      digitalWrite(relay1Pin, LOW);
      digitalWrite(relay2Pin, LOW);
      delay(500);  // Brief delay to show "Timer Ended" message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press Button");
    }
  }

  // Ultrasonic sensor to calculate fill percentage
  if (!timerRunning) {
    if (currentMillis - previousMillis >= 500) { // Non-blocking delay for ultrasonic reading
      previousMillis = currentMillis;

      // Trigger the ultrasonic sensor
      digitalWrite(trigPin, LOW);
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Read the echo with a timeout to avoid hanging
      duration = pulseIn(echoPin, HIGH, 25000);  // Timeout after 25 ms

      // Calculate distance to the object in inches
      distance = duration * 0.034 / 2 / 2.54;
 
      // Calculate the percentage of the container that is filled
      if (distance < containerHeight) {
        percentageFilled = round(((containerHeight - distance) / containerHeight) * 100);
      } else {
        percentageFilled = 0; // If sensor reads more than container height, assume empty
      }

      // Display "Press Button" on the top row and percentage fill on the bottom row
      lcd.setCursor(0, 0);
      lcd.print("Press Button    ");
      lcd.setCursor(0, 1);
      lcd.print("Filled: ");
      lcd.print(percentageFilled);
      lcd.print("%    ");

      // Trigger an alert if the container is 100% filled
      if (percentageFilled >= 100) {
        lcd.setCursor(0, 1);
        lcd.print("ALERT: FULL    "); // Clear rest of the line
      }
    }
  }

  // Reset button state when released
  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = false;  // Reset the button state when released
  }
}
