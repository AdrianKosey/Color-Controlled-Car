#include "TCS230.h"

TCS230::TCS230(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t out) : S0(s0), S1(s1), S2(s2), S3(s3), OUT(out)
{
}

void TCS230::begin()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(OUT, INPUT);
    digitalWrite(S0,LOW);
    digitalWrite(S1,HIGH);

    colors[0] = {205, 540, 140, true};    // Rojo
    colors[1] = {400, 300, 130, true};    // Verde
    colors[2] = {400, 310, 105, true};    // Azul
    colors[3] = {168, 265, 90, true};    // Amarillo
    colors[4] = {180, 225, 73, true};    // Blanco
    colors[5] = {1300, 1700, 590, true}; // Negro


    colors[0] = {21, 72, 59, true};    // Rojo
    colors[1] = {41, 25, 35, true};    // Verde
    colors[2] = {48, 30, 21, true};    // Azul
    colors[3] = {16, 28, 52, true};    // Amarillo
    colors[4] = {18, 20, 17, true};    // Blanco
    colors[5] = {190, 220, 180, true}; // Negro
}

int TCS230::getRed()
{
    // Set sensor to read Red only
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    // Define integer to represent Pulse Width
    int PW;
    // Read the output Pulse Width
    delayMicroseconds(200);
    PW = pulseIn(OUT, LOW);
    // Return the value
    return PW;
}

int TCS230::getGreen()
{
    // Set sensor to read Green only
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    // Define integer to represent Pulse Width
    int PW;
    delayMicroseconds(200);
    // Read the output Pulse Width
    PW = pulseIn(OUT, LOW);
    // Return the value
    return PW;
}

int TCS230::getBlue()
{
    // Set sensor to read Blue only
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    // Define integer to represent Pulse Width
    int PW;
    delayMicroseconds(200);
    // Read the output Pulse Width
    PW = pulseIn(OUT, LOW);
    // Return the value
    return PW;
}

ColorSample TCS230::readRGB()
{

    ColorSample c;

    c.r = getRed();
    c.g = getGreen();
    c.b = getBlue();

    Serial.print("R: ");
    Serial.println(c.r );
    Serial.print("G: ");
    Serial.println(c.g);    
    Serial.print("B: ");
    Serial.println(c.b);

    return c;
}

void TCS230::calibrateColor(uint8_t id)
{
    if (id >= MAX_COLORS)
        return;

    long r = 0, g = 0, b = 0;
    const uint8_t samples = 10;

    for (uint8_t i = 0; i < samples; i++)
    {
        r += getRed();
        g += getGreen();
        b += getBlue();
        delay(20);
    }

    colors[id].r = r / samples;
    colors[id].g = g / samples;
    colors[id].b = b / samples;
    colors[id].calibrated = true;
}


ColorSample TCS230::readNormalizedRGB()
{
    // 1. Obtener valores crudos (periodos)
    // Nota: Si un canal da 0, pulseIn falló (timeout)
    float r = getRed();
    float g = getGreen();
    float b = getBlue();

    // 2. Convertir Periodo -> Frecuencia (Intensidad)
    // Al usar 1.0/x, el color con más presencia ahora tendrá el valor más alto
    if (r == 0)
        r = 1; // Evitar división por cero
    if (g == 0)
        g = 1;
    if (b == 0)
        b = 1;

    float invR = 1000.0 / r;
    float invG = 1000.0 / g;
    float invB = 1000.0 / b;

    float sum = invR + invG + invB;

    ColorSample c;
    if (sum == 0)
    {
        c.r = c.g = c.b = 0;
        return c;
    }

    // 3. Normalizar (Valores de 0.0 a 1.0)
    c.r = invR / sum;
    c.g = invG / sum;
    c.b = invB / sum;

    return c;
}

int TCS230::detectColor()
{
    ColorSample current = readRGB();
    // ColorSample current = readNormalizedRGB(); basura
    int best = -1;
    long minError = 100000;

    for (int i = 0; i < MAX_COLORS; i++)
    {
        if (!colors[i].calibrated)
            continue;

        // Suma de diferencias absolutas
        long error = abs(current.r - (int)colors[i].r) +
                     abs(current.g - (int)colors[i].g) +
                     abs(current.b - (int)colors[i].b);

        if (error < minError)
        {
            minError = error;
            best = i;
        }
    }

    // Opcional: Si el error es demasiado grande, no es ningún color conocido
    if (minError > 500)
        return -1;

    return best;
}