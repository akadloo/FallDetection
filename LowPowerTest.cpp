#include "STM32LowPower.h"
#include "Arduino.h"

#define PIR_PIN D11 // Replace A0 with the GPIO pin connected to the PIR sensor
#define GATE_PIN D10 // Pin that will turn on gate to power Camera/radar
#define LED_PIN LED_BUILTIN

volatile bool personDetected = false; // Flag to track if PIR sensor was triggered

// Forward declaration of the ISR function
void wakeUpISR() {
  personDetected = true; // Set the flag when PIR sensor triggers
}

void setup() {
  pinMode(PIR_PIN, INPUT); // Configure PIR_PIN as input
  pinMode(LED_PIN, OUTPUT);
  pinMode(GATE_PIN, OUTPUT);

  // Ensure the outputs are off initially
  digitalWrite(GATE_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Configure low power
  LowPower.begin();

  // Attach an interrupt to the PIR_PIN, triggering on a rising edge (motion detected)
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), wakeUpISR, RISING);
}

void loop() {
  // Check if the personDetected flag is set
  if (personDetected) {
    digitalWrite(LED_PIN, HIGH);    // Turn on the LED as a visual cue
    digitalWrite(GATE_PIN, HIGH);   // Turn on the power switch to camera/radar
    delay(60000);                   // Keep the power switch on for 60 seconds
    digitalWrite(LED_PIN, LOW);     // Turn off the LED
    digitalWrite(GATE_PIN, LOW);    // Turn off the power switch

    personDetected = false;        // Reset the flag to wait for the next interrupt
  }

  // Go into deep sleep mode and wait for the PIR interrupt
  LowPower.deepSleep();  // Sleep until the next interrupt (PIR motion detected)
}
