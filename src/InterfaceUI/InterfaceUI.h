#pragma once
#include "ButtonUI.h"
#include "../color/TCS230.h"
#include "../motor/MotorDriver.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../config/default.h"
enum UIState
{
    UI_MENU,
    UI_START,
    UI_VIEW_COLORS,
    UI_VIEW_COLOR_ACTUAL,
    UI_VIEW_GIROSCOPIO
};
enum RobotAction
{
    ACTION_FORWARD,     // Rojo
    ACTION_BACKWARD,    // Verde
    ACTION_RIGHT,       // Azul
    ACTION_LEFT,        // Amarillo
    ACTION_STOP,        // Blanco
    ACTION_SPIN         // Negro
};

class InterfaceUI
{

private:
    ButtonUI &button;
    Adafruit_SSD1306 &display;
    TCS230 &sensor;
    MotorDriver &motors;
    UIState currentState;
    uint8_t selectedIndex;
    uint8_t menuLength;

    struct MenuItem
    {
        const char *label;
        UIState targetState;
    };
    static const MenuItem menuItems[];
    bool needsRedraw;

    static const char *colorMenu[7];

    // WRONG: RobotAction is actually a enum, so making an array doesnt have sense
    // RobotAction colorActions[6];
    uint8_t colorIndex;
    uint8_t scrollOffset;

    static const uint8_t visibleItems = 5;

    void drawCurrentScreen();

public:
    InterfaceUI(Adafruit_SSD1306 &oled, ButtonUI &btn, TCS230 &colorSensor, MotorDriver &motor);
    void begin();
    void update();
    void ui_nextItem();
    void ui_select();
    UIState ui_getState();
    void executeAction(RobotAction action);
};
