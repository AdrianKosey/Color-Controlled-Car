#include "InterfaceUI.h"
#include "math.h"

const char *InterfaceUI::colorMenu[7] = {
    "Rojo",
    "Verde",
    "Azul",
    "Amarillo",
    "Blanco",
    "Negro",
    "Volver"};

const char *actionNames[6] =
    {
        "Derecha",
        "CambiarV",
        "Izquierda",
        "CambiarV",
        "Adelante"
        "Detener",
};

InterfaceUI::InterfaceUI(Adafruit_SSD1306 &oled, TCS230 &colorSensor, MotorDriver &motor, Ultrasonic &ultrasonic)
    : display(oled), sensor(colorSensor), motors(motor), ultrasonic(ultrasonic)
{

    selectedIndex = 0;
    colorIndex = 0;
    scrollOffset = 0;
    motorTimer = 0;
    motorIsFast = false;
    historyIdx = 0;
    motorModeInitialized = false;
    for (int i = 0; i < 128; i++)
        gyroHistory[i] = 0;
}

void InterfaceUI::update()
{
    static unsigned long lastColorRead = 0;
    if (!motorModeInitialized)
    {
        motorModeInitialized = true;

        motors.stop();
        // 60 works good with full battery
        motors.setSpeed(100);
        lastColorRead = millis();

    }
    else
    {
        if (millis() - lastColorRead > 100)
        {
            lastColorRead = millis();
            // Leectura de sensor
            int color = sensor.detectColor();

            // Si es el mismo color, nada que hacer
            if (currentColor == color)
                return;

            if (color != -1)
            {
                // Detener motores y mapear 100 veces colores
                motors.stop();
                float bestColor = 0;
                for (int i = 0; i < 20; i++)
                {
                    bestColor += sensor.detectColor();
                    delay(20);
                }
                bestColor /= 20.0;
                bestColor = round(bestColor);

                currentColor = (int)bestColor;

                // Si no detecto color, hace blanco, avanzar
                if (currentColor == -1)
                    currentColor = 4;
                // TESTING
                Serial.print("Color: ");
                Serial.println(currentColor);

                Serial.println(colorMenu[currentColor]);
                Serial.println(actionNames[currentColor]);

                currentAction = (RobotAction)color;
                executeAction(currentAction);             
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

void InterfaceUI::executeAction(RobotAction action)
{
    switch (action)
    {
    case ACTION_FORWARD:
        Serial.println("EXECUTE FORWARD");
        motors.forward();
        break;

    case ACTION_RIGHT:
        // motors.right();

        Serial.println("EXECUTE RIGHT");

        motors.setSpeed(120);
        motors.rotateDegreesCW(90);
        break;

    case ACTION_LEFT:
        // motors.left();

        Serial.println("EXECUTE LEFT");
        motors.setSpeed(120);
        motors.rotateDegreesCCW(105);
        break;

    case ACTION_STOP:

        Serial.println("EXECUTE STOP");
        motors.stop();
        delay(5000);
        break;

    case ACTION_BACKWARD:
        motors.spin();
        break;

    case ACTION_TOGGLEV:

        Serial.println("EXECUTE TOGGLE ");
        motorIsFast = !motorIsFast;
        motors.setSpeed(motorIsFast ? 120 : 75);
        motors.forward();
        delay(200);
        break;
    }
}