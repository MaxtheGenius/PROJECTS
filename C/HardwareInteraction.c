#include <stdio.h>
#include <Arduino.h>

// Pin definition for the LED
#define LED_PIN 13 // LED is connected to digital pin 13

// Function prototypes
void initializeLED();
void turnOnLED();
void turnOffLED();
void blinkLED(int delayTime);

int main()
{
    // Initialize the LED
    initializeLED();

    // Loop to blink the LED every second
    while (1)
    {
        blinkLED(1000); // Blink LED every 1 second (1000ms)
    }

    return 0;
}

// Initialize the LED pin as an OUTPUT
void initializeLED()
{
    // Arduino setup function - Configure the LED pin as output
    pinMode(LED_PIN, OUTPUT);
}

// Turn the LED on by setting the LED pin HIGH
void turnOnLED()
{
    digitalWrite(LED_PIN, HIGH); // Set the LED pin to HIGH to turn it on
}

// Turn the LED off by setting the LED pin LOW
void turnOffLED()
{
    digitalWrite(LED_PIN, LOW); // Set the LED pin to LOW to turn it off
}

// Blink the LED by turning it on, waiting for a specified time, and turning it off
void blinkLED(int delayTime)
{
    turnOnLED();      // Turn on the LED
    delay(delayTime); // Wait for the given delay time
    turnOffLED();     // Turn off the LED
    delay(delayTime); // Wait for the same delay time again before blinking again
}