#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t echoPin, uint8_t trigPin)
{
    this->echoPin = echoPin;
    this->trigPin = trigPin;
}

void Ultrasonic::begin(void)
{
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

float Ultrasonic::getDistance(void)
{
    digitalWrite(trigPin, LOW);  // Set trig pin to low to ensure a clean pulse
    delayMicroseconds(2);        // Delay for 2 microseconds
    digitalWrite(trigPin, HIGH); // Send a 10 microsecond pulse by setting trig pin to high
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW); // Set trig pin back to low

    // Measure the pulse width of the echo pin and calculate the distance value
    float distance = pulseIn(echoPin, HIGH) / 58.00; // Formula: (340m/s * 1us) / 2
    return distance;
}