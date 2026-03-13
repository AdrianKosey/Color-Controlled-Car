#pragma once
#include <Arduino.h>
enum class ButtonEvent 
{
        NONE,
        SHORT_PRESS,
        LONG_PRESS
};


class ButtonUI
{
private:
    uint8_t pin;
    bool inverted;

    unsigned long pressStartTime;
    bool lastButtonState;
    bool longPressDetected;
public:
    ButtonUI(uint8_t p, bool in = false);
    void begin();
    bool isTriggered();
    ButtonEvent handleButton();
};
