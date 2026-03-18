#pragma once
#include <Arduino.h>

class MotorDriver
{
private:
    int in1,in2,in3,in4,ena,enb;

public:

    MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb);

    void begin();

    void forward();
    void backward();
    void right();
    void left();
    void stop();
    void spin();
};