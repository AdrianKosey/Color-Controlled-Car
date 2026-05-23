#include "MotorDriver.h"

MotorDriver::MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb)
    : in1(in1), in2(in2), in3(in3), in4(in4), ena(ena), enb(enb)
{
    currentSpeed = 0;
}

void MotorDriver::begin()
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(ena, OUTPUT);
    pinMode(enb, OUTPUT);

    // Start giroscope

    if (!IMU.begin())
    {
        Serial.println("Failed to initialize IMU!");
    }

    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println(" Hz");

    anguloAcumulado = 0;
    tiempoPrevio = 0;

    stop();
}

void MotorDriver::setSpeed(int speed)
{
    currentSpeed = constrain(speed, 0, 150);

    analogWrite(ena, constrain(currentSpeed, 0, 150));
    analogWrite(enb, constrain(currentSpeed + 30, 0, 150));
}

// Movimiento

void MotorDriver::forward()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void MotorDriver::backward()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void MotorDriver::stop()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    analogWrite(ena, 0);
    analogWrite(enb, 0);
}

void MotorDriver::right()
{

    anguloAcumulado = 0;
    tiempoPrevio = 0;

    while (abs(anguloAcumulado <= 90 - ANGLE_FIX))
    {

        // Esperar a que haya datos del giroscopio disponibles
        if (IMU.gyroscopeAvailable())
        {
            IMU.readGyroscope(gx, gy, gz);

            unsigned long tiempoActual = millis();
            float tiempoDelta = (tiempoActual - tiempoPrevio) / 1000.0; // en segundos
            tiempoPrevio = tiempoActual;

            // Asumimos que el giro es sobre el eje Z (en grados por segundo)
            float velocidadAngular = gz;

            // Filtro simple para ignorar el ruido cerca de 0
            if (abs(velocidadAngular) > 0.5)
            {
                anguloAcumulado += velocidadAngular * tiempoDelta;
            }

            // Ejecutar giro a la derecha
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
        }
    }
}

void MotorDriver::left()
{

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void MotorDriver::spin()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}