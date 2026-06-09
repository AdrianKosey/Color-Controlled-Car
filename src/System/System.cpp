#include "System.h"
#include "math.h"


System::System(TCS230 &colorSensor, MotorDriver &motor, Ultrasonic &ultrasonic)
    : sensor(colorSensor), motors(motor), ultrasonic(ultrasonic)
{
    scrollOffset = 0;
    motorTimer = 0;
    motorIsFast = false;
    historyIdx = 0;
    motorModeInitialized = false;
    for (int i = 0; i < 128; i++)
        gyroHistory[i] = 0;
}


void System::update()
{
    static unsigned long lastColorRead = 0;
    if (!motorModeInitialized)
    {
        motorModeInitialized = true;

        motors.stop();
        // 60 works good with full battery
        motors.setSpeed(75);
        lastColorRead = millis();

        actionInProgress = false;
    }

    // SI HAY ACCIÓN EN CURSO Y NO ES AVANZAR (BLOQUEO TEMPORAL)
    if (actionInProgress)
    {
        if (millis() - actionStartTime >= ACTION_DURATION)
        {
            actionInProgress = false;
            motors.setSpeed(75);
            motors.stop();
        }
    }

    else
    {
        if (millis() - lastColorRead > 100)
        {
            lastColorRead = millis();
            // Lee sensor y detecta cambio
            int color = sensor.detectColor();

            // Si es el mismo color, nada que hacer
            if (currentColor == color)
                return;

            if (color != -1)
            {
                // Detener motores y mapear 100 veces colores
                motors.stop();
                float bestColor = 0;
                for (int i = 0; i < 100; i++)
                {
                    bestColor += sensor.detectColor();
                }
                bestColor /= 100.0;
                bestColor = round(bestColor);

                currentColor = (int)bestColor;
                // TESTING
                Serial.print("Color: ");
                Serial.println(currentColor);

                Serial.println(colorMenu[currentColor]);

                currentAction = (RobotAction)color;
                if (currentAction == ACTION_FORWARD)
                {
                    executeAction(currentAction);
                    actionInProgress = false; // No bloqueamos
                }
                else
                {
                    executeAction(currentAction);
                    actionStartTime = millis(); // Iniciamos temporizador
                    actionInProgress = true;    // Bloqueamos hasta que pase el tiempo
                }
                lastColorSample = sensor.readRGB();                
            }
            else
            {
                currentColor = -1;
                currentAction = ACTION_STOP;
                motors.stop();
            }
        }
    }
}

void System::executeAction(RobotAction action)
{
    switch (action)
    {
    case ACTION_FORWARD:
        motors.forward();
        break;

    case ACTION_RIGHT:
        motors.right();
        // delay(2000);
        break;

    case ACTION_LEFT:
        motors.left();
        // delay(2000);
        break;

    case ACTION_STOP:
        motors.stop();
        delay(5000);
        break;

    case ACTION_BACKWARD:
        motors.spin();
        break;

    case ACTION_TOGGLEV:
        motorIsFast = !motorIsFast;
        motors.setSpeed(motorIsFast ? 120 : 75);
        motors.forward();
        break;
    }
}