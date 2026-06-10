#pragma once
#include <WebSocketsServer.h>
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

class SystemManager
{

private:
    TCS230 &colorSensor;
    MotorDriver &motors;
    Ultrasonic ultrasonic;
    WebSocketsServer &webSocket;

    bool actionInProgress = false;
    unsigned long actionStartTime = 0;      

    RobotAction currentAction = ACTION_STOP;
    ColorSample lastColorSample;    
    int currentColor = -1;
    bool motorIsFast;

    int rightTurns = 0;
    int leftTurns = 0;
    int obstacleCount = 0;
    bool wasObstacleDetected = false;

public:
    SystemManager(TCS230 &colorSensor, MotorDriver &motor, Ultrasonic &ultrasonic, WebSocketsServer &webSocket);
    void executeAction(RobotAction action);
    void update();
};
