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
    digitalWrite(S0, LOW);
    digitalWrite(S1, HIGH);

    colors[0] = {26, 119, 89, true};   // Rojo
    colors[1] = {151, 76, 88, true};   // Verde
    colors[2] = {162, 142, 85, true};  // Azul
    colors[3] = {19, 32, 62, true};    // Amarillo
    colors[4] = {20, 22, 18, true};    // Blanco
    colors[5] = {190, 209, 173, true}; // Negro
}

int TCS230::getRed()
{
    // Set sensor to read Red only
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    // Define integer to represent Pulse Width
    long totalPW = 0;
    for (int i = 0; i < 3; i++)
    {                                        // Take 3 quick samples
        noInterrupts();                      // Pause ESP32 background tasks
        totalPW += pulseIn(OUT, LOW, 20000); // Added a 20ms timeout!
        interrupts();                        // Resume background tasks
    }

    return totalPW / 3;
}

int TCS230::getGreen()
{
    // Set sensor to read Green only
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    // Define integer to represent Pulse Width
    long totalPW = 0;
    for (int i = 0; i < 3; i++)
    {                                        // Take 3 quick samples
        noInterrupts();                      // Pause ESP32 background tasks
        totalPW += pulseIn(OUT, LOW, 20000); // Added a 20ms timeout!
        interrupts();                        // Resume background tasks
    }

    return totalPW / 3;
}
int TCS230::getBlue()
{
    // Set sensor to read Blue only
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    long totalPW = 0;
    for (int i = 0; i < 3; i++)
    {                                        // Take 3 quick samples
        noInterrupts();                      // Pause ESP32 background tasks
        totalPW += pulseIn(OUT, LOW, 20000); // Added a 20ms timeout!
        interrupts();                        // Resume background tasks
    }

    return totalPW / 3;
}

ColorSample TCS230::readRGB()
{

    ColorSample c;

    c.r = getRed();
    c.g = getGreen();
    c.b = getBlue();

    Serial.print("R: ");
    Serial.println(c.r);
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

int TCS230::detectColor()
{
    ColorSample current = readRGB();
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