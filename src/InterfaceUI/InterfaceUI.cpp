#include "InterfaceUI.h"

const InterfaceUI::MenuItem InterfaceUI::menuItems[] = {
    {"Iniciar Recorrido", UI_START},
    {"Calibrar Colores", UI_VIEW_COLORS},
    {"Ver Color", UI_VIEW_COLOR_ACTUAL},
    {"Reset Giroscopio", UI_VIEW_GIROSCOPIO},
    {"Motores", UI_VIEW_MOTOR},

};

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
        "Avanzar",
        "Retroceder",
        "Derecha",
        "Izquierda",
        "Detener",
        "Rotar"};

// NO NEEDED BECAUSE RobotActions is an ENUM
// RobotAction colorActions[6] =
//     {
//         ACTION_FORWARD,
//         ACTION_BACKWARD,
//         ACTION_RIGHT,
//         ACTION_LEFT,
//         ACTION_STOP,
//         ACTION_SPIN};

InterfaceUI::InterfaceUI(Adafruit_SSD1306 &oled, ButtonUI &btn, TCS230 &colorSensor, MotorDriver &motor)
    : button(btn), display(oled), sensor(colorSensor), motors(motor)
{
    menuLength = sizeof(menuItems) / sizeof(menuItems[0]);

    currentState = UI_MENU;
    selectedIndex = 0;
    needsRedraw = true;
    colorIndex = 0;
    scrollOffset = 0;
    motorTimer = 0;
    motorIsFast = false;
    historyIdx = 0;
    motorModeInitialized = false;
    for (int i = 0; i < 128; i++)
        gyroHistory[i] = 0;
}

void InterfaceUI::begin()
{

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println("OLED BEGIN FAILED");
    }
    else
    {
        display.clearDisplay();
        display.display();
    }
}

void InterfaceUI::update()
{

    motors.update();
    ButtonEvent evt = button.handleButton();
    if (currentState == UI_MENU)
    {
        if (evt == ButtonEvent::SHORT_PRESS)
        {
            ui_nextItem();
            needsRedraw = true;
        }
        else if (evt == ButtonEvent::LONG_PRESS)
        {
            ui_select();
            needsRedraw = true;
        }
    }
    else if (currentState == UI_START)
    {
        static unsigned long lastColorRead = 0;
        if (!motorModeInitialized)
        {
            motorModeInitialized = true;

            motors.stop();
            motors.resetHeading();
            motors.setSpeed(200);
            lastColorRead = millis();

            actionInProgress = false;
        }

        // SI HAY ACCIÓN EN CURSO Y NO ES AVANZAR (BLOQUEO TEMPORAL)
        if (actionInProgress)
        {
            if (millis() - actionStartTime >= ACTION_DURATION)
            {
                actionInProgress = false;
                // Opcional: Si quieres que se detenga al terminar los 2s:
                // motors.stop();
            }
        }

        else
        {
            if (millis() - lastColorRead > 50)
            {
                lastColorRead = millis();
                int color = sensor.detectColor();

                if (color != -1)
                {
                    currentColor = color;
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
                    needsRedraw = true;
                }
                else
                {
                    currentColor = -1;
                    currentAction = ACTION_STOP;
                    motors.stop();
                }

                needsRedraw = true;
            }
        }
    }
    else if (currentState == UI_VIEW_COLORS)
    {
        if (evt == ButtonEvent::SHORT_PRESS)
        {
            ui_nextItem();
            needsRedraw = true;
        }
        else if (evt == ButtonEvent::LONG_PRESS)
        {
            if (colorIndex == 6)
            {
                currentState = UI_MENU;
                needsRedraw = true;
            }
            else
            {
                Serial.print("Calibrando color: ");
                Serial.println(colorIndex);
                display.clearDisplay();
                display.setCursor(0, 0);
                display.println("Calibrando Color: ");
                display.println(colorMenu[colorIndex]);
                display.display();
                sensor.calibrateColor(colorIndex);
                ColorSample colorRead = sensor.readRGB();
                Serial.print("R: ");
                Serial.println(colorRead.r);
                Serial.print("G: ");
                Serial.println(colorRead.g);
                Serial.print("B: ");
                Serial.println(colorRead.b);
                needsRedraw = true;
            }
        }
    }
    else if (currentState == UI_VIEW_MOTOR)
    {
        if (!motorModeInitialized)
        {
            motorModeInitialized = true;

            motors.stop();
            motors.resetHeading();
            motors.setSpeed(200);
            motors.forward();

            motorTimer = millis();
        }

        if (millis() - motorTimer > 3000)
        {
            motorTimer = millis();
            motorIsFast = !motorIsFast;
            motors.setSpeed(motorIsFast ? 255 : 200);
        }
        if (evt == ButtonEvent::LONG_PRESS)
        {
            motors.stop();
            currentState = UI_MENU;
            needsRedraw = true;
        }
        needsRedraw = true;
    }
    else if (currentState == UI_VIEW_GIROSCOPIO)
    {
        // Valor eje z
        float gz = motors.getGyroZ();

        // mapeamos
        // valor 0 centrado
        int8_t mappedValue = (int8_t)constrain(gz / 4, -15, 15);

        gyroHistory[historyIdx] = mappedValue;
        historyIdx = (historyIdx + 1) % 128; // Buffer circular

        // redibujo
        needsRedraw = true;

        if (evt == ButtonEvent::SHORT_PRESS)
        {
            motors.resetHeading();
        }
        else if (evt == ButtonEvent::LONG_PRESS)
        {
            currentState = UI_MENU;
        }
    }
    else
    {
        if (evt == ButtonEvent::LONG_PRESS)
        {
            motorModeInitialized = false;
            motors.stop();
            ui_select(); // volver al menú
            needsRedraw = true;
        }
    }

    if (needsRedraw)
    {
        needsRedraw = false;
        drawCurrentScreen();
    }
}

void InterfaceUI::drawCurrentScreen()
{
    display.clearDisplay();

    if (currentState == UI_MENU)
    {
        for (uint8_t i = 0; i < menuLength; i++)
        {
            int y = i * 12;
            if (i == selectedIndex)
            {
                display.fillRect(0, y, 128, 12, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
            }
            else
            {
                display.setTextColor(SSD1306_WHITE);
            }
            display.setCursor(5, y + 2);
            display.print(menuItems[i].label);
        }
        display.display();
    }
    else
    {
        switch (currentState)
        {
        case UI_START:
        {
            display.clearDisplay();
            display.setCursor(0, 0);

            display.println("Modo START");

            if (currentColor != -1)
            {
                display.print("IND: ");
                display.println(currentAction);

                display.print("Color: ");
                display.println(colorMenu[currentColor]);

                display.print("Accion:");
                display.println(actionNames[currentAction]);

                display.print("R: ");
                display.println(lastColorSample.r);

                display.print("G: ");
                display.println(lastColorSample.g);

                display.print("B: ");
                display.println(lastColorSample.b);
            }
            else
            {
                display.println("Color no conocido");
            }

            display.display();
        }
        break;
        case UI_VIEW_COLORS:
        {
            display.setCursor(0, 0);
            for (uint8_t i = 0; i < visibleItems; i++)
            {
                uint8_t itemIndex = scrollOffset + i;

                if (itemIndex >= 7)
                    break;

                int y = i * 12;

                if (itemIndex == colorIndex)
                {
                    display.fillRect(0, y, 118, 12, SSD1306_WHITE);
                    display.setTextColor(SSD1306_BLACK);
                }
                else
                {
                    display.setTextColor(SSD1306_WHITE);
                }

                display.setCursor(5, y + 2);
                display.print(colorMenu[itemIndex]);
            }
            // SCROLL
            int barHeight = (visibleItems * 40) / 7;
            int barY = (scrollOffset * 40) / 7;

            display.drawRect(120, 12, 6, 40, SSD1306_WHITE);
            display.fillRect(121, 12 + barY, 4, barHeight, SSD1306_WHITE);
        }
        break;
        case UI_VIEW_COLOR_ACTUAL:
        {
            display.setCursor(0, 0);
            display.println("Color Detectado");
            int indexColorLeido = sensor.detectColor();

            Serial.print("Color leido: ");
            Serial.println(indexColorLeido);
            display.print("Color: ");
            display.println(colorMenu[indexColorLeido]);

            delay(2000);
        }
        break;
        case UI_VIEW_GIROSCOPIO:
        {
            display.setCursor(0, 0);
            display.println("--- MONITOR GYRO ---");

            // Linea base
            display.drawFastHLine(0, 45, 128, SSD1306_WHITE);

            // onda
            for (int x = 0; x < 127; x++)
            {
                // indices para el punto actual y el siguiente
                uint8_t i1 = (historyIdx + x) % 128;
                uint8_t i2 = (historyIdx + x + 1) % 128;

                // linea entre cada punto
                display.drawLine(
                    x, 45 - gyroHistory[i1],
                    x + 1, 45 - gyroHistory[i2],
                    SSD1306_WHITE);
            }

            display.setCursor(0, 12);
            display.print("Z: ");
            display.print(motors.getHeading(), 1); // angulo acumulado
            display.println(" deg");
        }
        break;

        case UI_VIEW_MOTOR:
        {
            display.clearDisplay();
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.println("MODO GIROSCOPIO");
            display.setCursor(0, 12);
            display.print("Velocidad: ");
            display.println(motorIsFast ? "Rapido" : "Lento");

            // Heading actual
            display.setCursor(0, 24);
            display.print("Heading: ");
            display.print(motors.getHeading(), 1);
            display.println(" deg");

            // Velocidad angular
            display.setCursor(0, 36);
            display.print("Gyro Z: ");
            display.print(motors.getGyroZ(), 1);
            display.setCursor(0, 52);
            display.print("Long: Salir");

            display.display();
        }
        break;

        default:
            break;
        }
    }

    display.display();
}

void InterfaceUI::ui_nextItem()
{
    if (currentState == UI_MENU)
    {
        selectedIndex++;
        if (selectedIndex >= menuLength)
            selectedIndex = 0;
    }
    else if (currentState == UI_VIEW_COLORS)
    {
        colorIndex++;

        if (colorIndex >= 7)
            colorIndex = 0;

        if (colorIndex >= scrollOffset + visibleItems)
            scrollOffset++;

        if (colorIndex < scrollOffset)
            scrollOffset = colorIndex;
    }
}

void InterfaceUI::ui_select()
{
    if (currentState == UI_MENU)
        currentState = menuItems[selectedIndex].targetState;
    else
        currentState = UI_MENU;
}

UIState InterfaceUI::ui_getState()
{
    return currentState;
}

void InterfaceUI::executeAction(RobotAction action)
{
    switch (action)
    {
    case ACTION_FORWARD:
        motors.forward();

        break;

    case ACTION_BACKWARD:
        motors.backward();
        // delay(2000);
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
        // delay(2000);
        break;

    case ACTION_SPIN:
        motors.spin();
        // delay(2000);
        break;
    }
}