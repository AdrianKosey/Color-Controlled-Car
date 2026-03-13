#include "MotorDriver.h"

MotorDriver::MotorDriver(int a,int b,int c,int d,int e,int f)
{
    in1=a; in2=b; in3=c; in4=d; ena=e; enb=f;
}

void MotorDriver::begin()
{
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(in3,OUTPUT);
    pinMode(in4,OUTPUT);
    pinMode(ena,OUTPUT);
    pinMode(enb,OUTPUT);

    digitalWrite(ena,HIGH);
    digitalWrite(enb,HIGH);
}
void MotorDriver::forward()
{
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
}

void MotorDriver::backward()
{
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
}

void MotorDriver::right()
{
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
}

void MotorDriver::left()
{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
}

void MotorDriver::stop()
{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
}

void MotorDriver::spin()
{
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
}