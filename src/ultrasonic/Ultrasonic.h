#pragma once
#include <Arduino.h>
class Ultrasonic
{
private:
    uint8_t echoPin;
    uint8_t trigPin;

public:
    Ultrasonic(uint8_t, uint8_t);
    void begin(void);
    float getDistance(void);
};
