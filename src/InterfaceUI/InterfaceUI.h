#pragma once
#include "../color/TCS230.h"
#include "../motor/MotorDriver.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../config/default.h"
#include "../ultrasonic/Ultrasonic.h"

enum RobotAction
{
    ACTION_RIGHT,    // Rojo
    ACTION_TOGGLEV,   // Verde y Amarillo
    ACTION_LEFT,     // Azul
    ACTION_BACKWARD, // Amarillo
    ACTION_FORWARD,  // Blanco
    ACTION_STOP      // Negro
};

class InterfaceUI
{

private:
    Adafruit_SSD1306 &display;
    TCS230 &sensor;
    MotorDriver &motors;
    Ultrasonic ultrasonic;
    uint8_t selectedIndex;
    uint8_t menuLength;

    bool actionInProgress = false;

    bool needsRedraw;

    static const char *colorMenu[7];

    // WRONG: RobotAction is actually a enum, so making an array doesnt have sense
    // RobotAction colorActions[6];

    // Index for UI option
    uint8_t colorIndex;
    uint8_t scrollOffset;
    unsigned long motorTimer;
    bool motorIsFast;

    static const uint8_t visibleItems = 5;
    int8_t gyroHistory[128];
    uint8_t historyIdx;
    bool motorModeInitialized = false;
    void drawCurrentScreen();
    int currentColor = -1;
    RobotAction currentAction = ACTION_STOP;
    ColorSample lastColorSample;

public:
    InterfaceUI(Adafruit_SSD1306 &oled, TCS230 &colorSensor, MotorDriver &motor, Ultrasonic &ultrasonic);
    void begin();
    void update();
    void executeAction(RobotAction action);
};
