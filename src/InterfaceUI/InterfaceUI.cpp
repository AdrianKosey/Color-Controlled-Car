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
    motorIndex = 0;
    scrollOffset = 0;
    motorTimer = 0;
    motorIsFast = false;
    calA = 0.75f;
    calB = 1.0f;
    motorIndex = 0;
    motor.setCalibration(calA, calB);
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
    ButtonEvent evt = button.handleButton();

    if (currentState == UI_VIEW_MOTOR)
    {
        if (millis() - motorTimer > 3000)
        {
            motorTimer = millis();
            motorIsFast = !motorIsFast;
            motors.setSpeed(motorIsFast ? 200 : 100);
            motors.setCalibration(calA, calB);
            motors.forward();
            needsRedraw = true;
        }
    }

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
        if (evt == ButtonEvent::SHORT_PRESS)
        {
            if (motorIndex == 0)
            {
                calA += 0.05f;
                if (calA > 1.01f)
                    calA = 0.0f;
            }
            else if (motorIndex == 1)
            {
                calB += 0.05f;
                if (calB > 1.01f)
                    calB = 0.0f;
            }
            else if (motorIndex == 2)
            {
                motors.stop();
                currentState = UI_MENU;
            }
            motors.setCalibration(calA, calB);
            needsRedraw = true;
        }
        else if (evt == ButtonEvent::LONG_PRESS)
        {
            motorIndex++;
            if (motorIndex > 2)
                motorIndex = 0;

            // Si el nuevo index es salir, podrías elegir si parar o no
            if (motorIndex == 2)
            { /* Opcional: motores.stop(); */
            }

            needsRedraw = true;
        }
    }
    else
    {
        if (evt == ButtonEvent::LONG_PRESS)
        {
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
            Serial.println("DETECTANDO COLORES Y MOVIENDO MOTORES");

            int color = sensor.detectColor();

            // Valida color existente
            if (color != -1)
            {

                // "color" is actually the index, so making this change it isn't neccesary
                // RobotAction action = colorActions[color];
                RobotAction action = (RobotAction)color;

                executeAction(action);

                Serial.println(actionNames[action]);

                display.setCursor(0, 0);
                display.println("Modo START");

                display.print("IND: ");
                display.println(action);

                display.print("Color: ");
                display.println(colorMenu[color]);

                display.print("Accion:");
                display.println(actionNames[action]);

                ColorSample current = sensor.readRGB();
                display.print("R: ");
                display.println(current.r);
                display.print("G: ");
                display.println(current.g);
                display.print("B: ");
                display.println(current.b);

                delay(300);
            }
            else
            {

                display.setCursor(0, 0);
                display.print("Color no conocido");
                executeAction(ACTION_STOP);
            }

            needsRedraw = true;
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
            display.println("Giroscopio");
            display.println("Esperando accion...");
        }
        break;

        case UI_VIEW_MOTOR:
        {
            display.clearDisplay();
            display.setTextColor(SSD1306_WHITE);

            // Encabezado
            display.setCursor(0, 0);
            display.print("CALIB. MOTORES ");
            display.println(motorIsFast ? "Rapido" : "Lento");

            // Fila Motor A
            display.setCursor(0, 18);
            if (motorIndex == 0)
                display.print("> Motor A (Iz): ");
            else
             display.print("  Motor A (Iz): ");
            display.println(calA);

            // Fila Motor B
            display.setCursor(0, 30);
            if (motorIndex == 1)
                display.print("> Motor B (De): ");
            else
            display.print("  Motor B (De): ");
            display.println(calB);

            // Opción Salir
            display.setCursor(0, 45);
            if (motorIndex == 2)
                display.print("> ");
            display.print("[ Volver al Menu ]");


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
        motors.setSpeed(motorIsFast ? 200 : 100);
        motors.forward();
        break;

    case ACTION_BACKWARD:
        motors.setSpeed(motorIsFast ? 200 : 100);
        motors.backward();
        break;

    case ACTION_RIGHT:
        motors.setSpeed(motorIsFast ? 200 : 100);
        motors.right();
        break;

    case ACTION_LEFT:
        motors.setSpeed(motorIsFast ? 200 : 100);
        motors.left();
        break;

    case ACTION_STOP:
        motors.stop();
        break;

    case ACTION_SPIN:
        motorIsFast = !motorIsFast;
        motors.setSpeed(motorIsFast ? 200 : 100);
        motors.forward();
        break;
    }
}