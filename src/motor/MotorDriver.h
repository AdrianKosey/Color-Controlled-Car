#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>

class MotorDriver {
  private:
    int in1, in2, in3, in4, ena, enb;
    int currentSpeed;
    float factorA; 
    float factorB;

    void applySpeed(); 

  public:
    MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb);
    
    void begin();
    
    void setSpeed(int speed);
    void setCalibration(float calA, float calB);
    
    // Movimientos
    void forward();
    void backward();
    void right();
    void left();
    void stop();
    void spin();
};

#endif