#include <math.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire)
const int thermistorPin = 26;
const int R1 = 10900;
const float A = 0.0011384;
const float B = 0.00023245;
const float C = 0.00000009489;
// Pin connected to the relay
const int relayPin = 16;
// Push button pins
const int upButtonPin = 0;
const int downButtonPin = 3;
float setTemperature = 30.0; // Initial set temperature
void setup() {
pinMode(relayPin, OUTPUT);
pinMode(upButtonPin, INPUT);
pinMode(downButtonPin, INPUT);
digitalWrite(upButtonPin, LOW); // Enable internal pull-up resistor
digitalWrite(downButtonPin, LOW); // Enable internal pull-up resistor
Serial.begin(9600);
}
void loop() {
// Read ADC value
int adcValue = analogRead(thermistorPin);
// Calculate voltage
float voltage = (adcValue * 5.0) / 65535.0;
// Calculate resistance
float resistance = (R1 * voltage) / (5.0 - voltage);
// Calculate temperature using Steinhart-Hart equation
float logR2 = log(resistance);
float t = 1.0 / (A + B * logR2 + C * logR2 * logR2 * logR2);
float tc = (t - 273.15) - 137;
// Print temperature
Serial.print("Temperature: ");
Serial.print(tc);
Serial.println("°C");
Serial.print("Set Temperature: ");
Serial.println(setTemperature);
// GPIO pin number 3 for down button
// Read up button state
if (digitalRead(downButtonPin) == LOW && setTemperature > 0) {
setTemperature--; // Decrement set temperature
//delay(200); // Delay for button debouncing
//Serial.print("Set Temperature: ");
//Serial.println(setTemperature);
}
// Read down button state
if (digitalRead(upButtonPin) == LOW && setTemperature < 50) {
setTemperature++; // Increment set temperature
//delay(200); // Delay for button debouncing
//Serial.print("Set Temperature: ");
//Serial.println(setTemperature);
}
// Check if the temperature exceeds the set temperature
if (tc > setTemperature) {
digitalWrite(relayPin, HIGH); // Turn off the relay
Serial.println("Relay OFF");
} else {
digitalWrite(relayPin, LOW); // Turn on the relay
Serial.println("Relay ON");
}
// Delay before taking the next temperature reading
delay(100);
}


