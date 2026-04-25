#include "MotorDriver.h"

MotorDriver::MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb)
    : in1(in1), in2(in2), in3(in3), in4(in4), ena(ena), enb(enb)
{
    currentSpeed = 0;
    // Por defecto, ambos motores al 100% de la velocidad solicitada
    factorA = 1.0; 
    factorB = 1.0; 
}

void MotorDriver::begin()
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(ena, OUTPUT);
    pinMode(enb, OUTPUT);

    // Inicializar motores apagados
    stop();
}

// --- GESTIÓN DE VELOCIDAD ---

void MotorDriver::setCalibration(float calA, float calB) {
    // Permite ajustar las diferencias físicas de los motores.
    // Ej: Si el motor B es más rápido, usa setCalibration(1.0, 0.85);
    factorA = calA;
    factorB = calB;
    applySpeed(); // Actualiza la velocidad inmediatamente
}

void MotorDriver::setSpeed(int speed) {
    // Aseguramos que la velocidad base esté entre 0 y 255
    currentSpeed = constrain(speed, 0, 255);
    applySpeed();
}

void MotorDriver::applySpeed() {
    // Aplicamos el factor de calibración a cada motor
    int speedA = currentSpeed * factorA;
    int speedB = currentSpeed * factorB;
    
    analogWrite(ena, speedA);
    analogWrite(enb, speedB);
}

// --- LÓGICA DE MOVIMIENTO ---

void MotorDriver::forward()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    applySpeed();
}

void MotorDriver::backward()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    applySpeed();
}

void MotorDriver::right()
{
    // Para girar suave, un motor va adelante y otro se detiene o va más lento
    // Aquí hacemos giro sobre su propio eje (spin suave)
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    applySpeed();
}

void MotorDriver::left()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    applySpeed();
}

void MotorDriver::stop()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    // Apagamos el PWM para detener por completo
    analogWrite(ena, 0);
    analogWrite(enb, 0);
}

void MotorDriver::spin()
{
    // Clockwise spin sobre su propio eje
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    applySpeed();
}