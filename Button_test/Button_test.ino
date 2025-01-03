const int buttonPin = 2; // Button pin
int buttonState = 1;     // Variable to store button state

void setup() {
  Serial.begin(9600);    // Initialize serial monitor
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read the button state

  if (buttonState == LOW) {
    Serial.println("Button Pressed");  // Log message when button is pressed
  } else {
    Serial.println("Button Released"); // Log message when button is released
  }

  delay(200); // Debounce delay
}
