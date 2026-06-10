#include "MotorDriver.h"

MotorDriver::MotorDriver(int in1, int in2, int in3, int in4, int ena, int enb)
    : in1(in1), in2(in2), in3(in3), in4(in4), ena(ena), enb(enb)
{
    currentSpeed = 0;
    isFast = false;
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
    analogWrite(enb, constrain(currentSpeed + 27, 0, 150));
}

// Movimiento

void MotorDriver::forward()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Briefly set high speed
    oldSpeed = currentSpeed;
    setSpeed(130);
    delay(50);
    setSpeed(oldSpeed);
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
    oldSpeed = currentSpeed;
    setSpeed(120);

    anguloAcumulado = 0;
    tiempoPrevio = micros();

    while (1)
    {

        // Esperar a que haya datos del giroscopio disponibles
        if (IMU.gyroscopeAvailable())
        {
            IMU.readGyroscope(gx, gy, gz);

            unsigned long tiempoActual = micros();
            float tiempoDelta = (tiempoActual - tiempoPrevio) / 1000000.0;
            tiempoPrevio = tiempoActual;

            // Asumimos que el giro es sobre el eje Z (en grados por segundo)
            float velocidadAngular = gz;

            // Filtro simple para ignorar el ruido cerca de 0
            if (abs(velocidadAngular) > 0.5)
            {
                anguloAcumulado += velocidadAngular * tiempoDelta;
            }

            if (abs(anguloAcumulado) >= 90 - ANGLE_FIX)
                break;

            // Ejecutar giro a la derecha
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);

            Serial.println(anguloAcumulado);
        }
    }

    setSpeed(oldSpeed);
    stop();
}

void MotorDriver::left()
{

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void MotorDriver::toggleVelocity()
{
    isFast = !isFast;
    setSpeed(isFast ? 120 : 75);
    forward();
}

void MotorDriver::spin()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void MotorDriver::rotateDegreesCW(int degrees)
{
    oldSpeed = currentSpeed;
    setSpeed(115);

    anguloAcumulado = 0;
    tiempoPrevio = micros();

    while (1)
    {

        // Esperar a que haya datos del giroscopio disponibles
        if (IMU.gyroscopeAvailable())
        {
            IMU.readGyroscope(gx, gy, gz);

            unsigned long tiempoActual = micros();
            float tiempoDelta = (tiempoActual - tiempoPrevio) / 1000000.0;
            tiempoPrevio = tiempoActual;

            // Asumimos que el giro es sobre el eje Z (en grados por segundo)
            float velocidadAngular = gz;

            // Filtro simple para ignorar el ruido cerca de 0
            if (abs(velocidadAngular) > 0.5)
            {
                anguloAcumulado += velocidadAngular * tiempoDelta;
            }

            if (abs(anguloAcumulado) >= degrees - ANGLE_FIX)
                break;

            // Ejecutar giro a la derecha
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
        }
    }

    setSpeed(oldSpeed);
    stop();
}

void MotorDriver::rotateDegreesCCW(int degrees)
{
    oldSpeed = currentSpeed;
    setSpeed(115);

    anguloAcumulado = 0;
    tiempoPrevio = micros();

    while (1)
    {

        // Esperar a que haya datos del giroscopio disponibles
        if (IMU.gyroscopeAvailable())
        {
            IMU.readGyroscope(gx, gy, gz);

            unsigned long tiempoActual = micros();
            float tiempoDelta = (tiempoActual - tiempoPrevio) / 1000000.0;
            tiempoPrevio = tiempoActual;

            // Asumimos que el giro es sobre el eje Z (en grados por segundo)
            float velocidadAngular = gz;

            // Filtro simple para ignorar el ruido cerca de 0
            if (abs(velocidadAngular) > 0.5)
            {
                anguloAcumulado += velocidadAngular * tiempoDelta;
            }

            if (abs(anguloAcumulado) >= degrees - ANGLE_FIX * 2)
                break;

            // Ejecutar giro a la izquierda
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);
        }
    }

    setSpeed(oldSpeed);
    stop();
}