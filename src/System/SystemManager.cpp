#include "SystemManager.h"
#include "math.h"

SystemManager::SystemManager(TCS230 &colorSensor, MotorDriver &motor, Ultrasonic &ultrasonic)
    : colorSensor(colorSensor), motors(motor), ultrasonic(ultrasonic)
{
}

void SystemManager::update()
{

    static unsigned long lastColorRead = 0;
    // Lee color cada 200 milisegundos
    if (millis() - lastColorRead > 200)
    {
        // Lee colorSensor y detecta cambio
        int color = colorSensor.detectColor();
        lastColorRead = millis();

        // Si es el mismo color, nada que hacer
        if (currentColor == color)
            return;

        // Si color es -1 no se logró identificar cuál color es
        if (color != -1)
        {
            // Detener motores y mapear 10 veces colores
            motors.stop();
            float bestColor = 0;
            for (int i = 0; i < 10; i++)
            {
                bestColor += colorSensor.detectColor();
                delay(100);
            }
            bestColor /= 10.0;
            bestColor = round(bestColor);
            currentColor = (int)bestColor;
            // TESTING
            Serial.print("Color: ");
            Serial.println(colorSensor.ColorStrings[currentColor]);
            delay(5000);
            return;

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
            lastColorSample = colorSensor.readRGB();
        }
        else
        {
            currentColor = -1;
            currentAction = ACTION_STOP;
            motors.stop();
        }
    }
}

void SystemManager::executeAction(RobotAction action)
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