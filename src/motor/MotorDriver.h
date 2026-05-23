#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include <Arduino_LSM6DS3.h>

class MotorDriver
{
private:
  int in1, in2, in3, in4, ena, enb;
  int currentSpeed;
  void applySpeed();

  // Turn with Osciloscope
  float anguloAcumulado;
  unsigned long tiempoPrevio;
  float gx, gy, gz;

  // Restado a angulos contemplando inercia
  const int ANGLE_FIX = 16;

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
};

#endif