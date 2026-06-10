#pragma once
#include <Arduino.h>

enum Color
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE,
    BLACK,
    MAX_COLORS
};

struct ColorSample
{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    bool calibrated;
};

class TCS230
{
private:
    uint8_t S0, S1, S2, S3, OUT;
    int redPW = 0;
    int greenPW = 0;
    int bluePW = 0;
    ColorSample colors[MAX_COLORS];

public:
    TCS230(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t out);

    const char *ColorStrings[MAX_COLORS] = {
        "RED",
        "GREEN",
        "BLUE",
        "YELLOW",
        "WHITE",
        "BLACK"};
    void begin();
    int getRed();
    int getBlue();
    int getGreen();
    ColorSample readRGB();
    ColorSample readNormalizedRGB();
    void calibrateColor(uint8_t id);
    int detectColor();
    float calculateDistance(ColorSample a, ColorSample b);
};
