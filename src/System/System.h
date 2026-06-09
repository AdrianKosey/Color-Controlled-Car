#pragma once
#include "../color/TCS230.h"
#include "../motor/MotorDriver.h"
#include <Arduino.h>
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

class System
{

private:
    TCS230 &sensor;
    MotorDriver &motors;
    Ultrasonic ultrasonic;

    bool actionInProgress = false;
    unsigned long actionStartTime = 0;
    const unsigned long ACTION_DURATION = 2000; // 2 segundo
        

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
    System(TCS230 &colorSensor, MotorDriver &motor, Ultrasonic &ultrasonic);
    void executeAction(RobotAction action);
    void update();
};
