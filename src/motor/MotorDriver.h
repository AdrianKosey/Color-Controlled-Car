#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>
#include "../libs/SparkFun_LSM6DS3/src/SparkFunLSM6DS3.h" // Librería para el giroscopio LSM6DS3
#include <Wire.h>

class MotorDriver {
  private:
    int in1, in2, in3, in4, ena, enb;
    int currentSpeed;
    LSM6DS3 imu;
    float targetHeading;    // angulo objetivo
    float currentHeading;   // anngulo actual
    unsigned long lastTime;
    float kp, kd;
    bool isCorrectionActive;
    void updateHeading(); // velocidad y
    void applySpeed(); 

  public:
    MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb);
    
    void begin();
    
    void setSpeed(int speed);
    
    // Movimientos
    void forward();
    void backward();
    void right();
    void left();
    void stop();
    void spin();
    void update();
    float getHeading();
    float getGyroZ();
    void resetHeading();
};

#endif