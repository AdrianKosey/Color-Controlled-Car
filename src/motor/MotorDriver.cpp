#include "MotorDriver.h"

MotorDriver::MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb)
    : in1(in1), in2(in2), in3(in3), in4(in4), ena(ena), enb(enb), imu(I2C_MODE, 0x6A)
{
    currentSpeed = 0;

    currentHeading = 0;
    targetHeading = 0;

    kp = 3.0;
    kd = 1.0;

    isCorrectionActive = false;

    lastTime = millis();
}

void MotorDriver::begin()
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(ena, OUTPUT);
    pinMode(enb, OUTPUT);

    Wire.begin();

    if (imu.begin() != 0)
    {
        Serial.println("Error: LSM6DS3 no detectado.");
    }

    lastTime = millis();
    stop();
}

void MotorDriver::updateHeading()
{
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    float gyroZ = imu.readFloatGyroZ();

    // sensibilidad
    if (abs(gyroZ) > 0.5)
    {
        currentHeading += gyroZ * dt;
    }
}

void MotorDriver::update()
{
    updateHeading();

    if (isCorrectionActive)
    {
        applySpeed();
    }
}


float MotorDriver::getHeading() { return currentHeading; }

float MotorDriver::getGyroZ() { return imu.readFloatGyroZ(); }

void MotorDriver::resetHeading()
{
    currentHeading = 0;
}

void MotorDriver::applySpeed() {
    int speedA, speedB;

    if (isCorrectionActive) {

        float error = targetHeading - currentHeading;
        float gyroZ = imu.readFloatGyroZ();

        int correction = (error * kp) - (gyroZ * kd);
        correction = constrain(correction, -80, 80);

        int baseSpeed = currentSpeed;

        speedA = baseSpeed - correction;
        speedB = baseSpeed + correction;

    } else {
        speedA = currentSpeed;
        speedB = currentSpeed;
    }

    analogWrite(ena, constrain(speedA, 0, 255));
    analogWrite(enb, constrain(speedB, 0, 255));
}

void MotorDriver::setSpeed(int speed)
{
    currentSpeed = constrain(speed, 0, 255);
}

// Movimiento

void MotorDriver::forward()
{

    resetHeading();
    targetHeading = 0;
    isCorrectionActive = true;

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void MotorDriver::backward()
{
    isCorrectionActive = false;

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void MotorDriver::stop()
{
    isCorrectionActive = false;

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    analogWrite(ena, 0);
    analogWrite(enb, 0);
}

void MotorDriver::right()
{
    isCorrectionActive = false;

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void MotorDriver::left()
{
    isCorrectionActive = false;

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void MotorDriver::spin()
{
    isCorrectionActive = false;

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}