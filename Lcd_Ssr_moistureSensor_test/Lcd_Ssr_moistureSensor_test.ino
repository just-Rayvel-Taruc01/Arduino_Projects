const int relay1Pin = 8;  // Digital pin for channel 1
const int relay2Pin = 9;  // Digital pin for channel 2

void setup() {
  pinMode(relay1Pin, OUTPUT);  // Set relay1 pin as output
  pinMode(relay2Pin, OUTPUT);  // Set relay2 pin as output
}

void loop() {
  digitalWrite(relay1Pin, HIGH);  // Turn ON relay channel 1
  delay(2000);                    // Wait for 2 seconds
  digitalWrite(relay1Pin, LOW);   // Turn OFF relay channel 1
  delay(2000);                    // Wait for 2 seconds

  digitalWrite(relay2Pin, HIGH);  // Turn ON relay channel 2
  delay(2000);                    // Wait for 2 seconds
  digitalWrite(relay2Pin, LOW);   // Turn OFF relay channel 2
  delay(2000);                    // Wait for 2 seconds
}
