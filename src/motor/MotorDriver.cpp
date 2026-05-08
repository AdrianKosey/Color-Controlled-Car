#include "MotorDriver.h"

MotorDriver::MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb)
    : in1(in1), in2(in2), in3(in3), in4(in4), ena(ena), enb(enb), imu(I2C_MODE, 0x6A)
{
    currentSpeed = 0;

    currentHeading = 0;
    targetHeading = 0;

    kp = 3.0;
    kd = 1.5;

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
    calibrateGyro();
}

void MotorDriver::calibrateGyro()
{
    float sum = 0;
    int samples = 200;
    for (int i = 0; i < samples; i++)
    {
        sum += imu.readFloatGyroZ();
        delay(5);
    }
    gyroBiasZ = sum / samples;
    Serial.print("Bias Z:");
    Serial.println(gyroBiasZ);
}

void MotorDriver::updateHeading()
{
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    float gyroZ = imu.readFloatGyroZ() - gyroBiasZ;

    // sensibilidad
    if (abs(gyroZ) < 5)
    {
        gyroZ = 0;
    }
    currentHeading += gyroZ * dt;
}

float MotorDriver::getHeading() { return currentHeading; }

float MotorDriver::getGyroZ() { return imu.readFloatGyroZ(); }

void MotorDriver::resetHeading()
{
    currentHeading = 0;
}

void MotorDriver::applySpeed()
{
    int speedA, speedB;

    speedA = currentSpeed;
    speedB = currentSpeed;

    analogWrite(ena, constrain(speedA, 0, 150));
    analogWrite(enb, constrain(speedB, 0, 150 + 20));
}

void MotorDriver::setSpeed(int speed)
{
    currentSpeed = constrain(speed, 0, 150);
}

// Movimiento
// Arrancar motor
void MotorDriver::startUp(){   
    int normalSpeed = currentSpeed;

    setSpeed(140);  // High speed to initialize motors
    applySpeed();
    delay(50);

    setSpeed(normalSpeed);    
    applySpeed();
}

void MotorDriver::forward()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    startUp();

}

void MotorDriver::backward()
{

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    startUp();
}

void MotorDriver::stop()
{

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    analogWrite(ena, 0);
    analogWrite(enb, 0);

    // Como es giro, necesita algo más de potencia
    setSpeed(110);
    applySpeed();
}

void MotorDriver::right()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    startUp();
    // Como es giro, necesita algo más de potencia
    setSpeed(110);
    applySpeed();
}

void MotorDriver::left()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    startUp();
    // Como es giro, necesita algo más de potencia
    setSpeed(110);
    applySpeed();
}

void MotorDriver::spin()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
    startUp();  
}