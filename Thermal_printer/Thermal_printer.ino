#include <SoftwareSerial.h>

void setup() {
  Serial1.begin(9600);  // Initialize Serial1 for Mega to communicate with printer at 9600 baud rate
  delay(1000);  // Wait a moment for the printer to initialize
  printText("Hello, QR701 Thermal Printer!");
}

void loop() {
  // Add any logic if you need continuous printing or additional commands
}

void printText(const char* text) {
  Serial1.println(text);  // Send text to the printer
  delay(100);             // Small delay to ensure data is sent
}

void printLine() {
  Serial1.write(0x0A);  // Sends a line feed character to move the paper up
}
