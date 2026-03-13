#pragma once
#include <Arduino.h>

class MotorDriver
{
private:
    int in1,in2,in3,in4,ena,enb;

public:

    MotorDriver(int a,int b,int c,int d,int e,int f);

    void begin();

    void forward();
    void backward();
    void right();
    void left();
    void stop();
    void spin();
};