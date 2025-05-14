#include "STM32LowPower.h"
#include "Arduino.h"

#define GATE_PIN D10    // Pin that controls the power switch (gate)
#define PIR_PIN D11     // Pin connected to the PIR sensor output
#define LED_PIN LED_BUILTIN  // Built-in LED for visual feedback

volatile bool pirTriggered = false;  // Flag set by the ISR when PIR goes high
unsigned long startTime = 0;           // To record when the outputs were turned on
const unsigned long onDuration = 60000; // 60 seconds in milliseconds

// Interrupt Service Routine for the PIR sensor
void pirISR() {
  pirTriggered = true;
}

void setup() {
  pinMode(GATE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);  // Set PIR_PIN as input
  
  // Ensure the outputs are off initially
  digitalWrite(GATE_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  // Attach an interrupt to the PIR sensor pin on the rising edge
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
}

void loop() {
  if (pirTriggered) {
    // If this is the first time after the interrupt, start the timer
    if (startTime == 0) {
      startTime = millis();
      // Turn on the gate and LED
      digitalWrite(GATE_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
    }
    // Check if 30 seconds have elapsed
    if (millis() - startTime >= onDuration) {
      // Turn off the outputs
      digitalWrite(GATE_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      // Reset the trigger and timer so the system is ready for another event
      pirTriggered = false;
      startTime = 0;
    }
  }
  // Optionally, insert low-power or background tasks here
}
