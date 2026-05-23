#include <Arduino.h>
#include "InterfaceUI/InterfaceUI.h"
#include "InterfaceUI/ButtonUI.h"
#include "color/TCS230.h"
#include "config/default.h"
#include "motor/MotorDriver.h"
#include "ultrasonic/Ultrasonic.h"
#include <Arduino_LSM6DS3.h>
// OLED
TCS230 sensorColor(S0, S1, S2, S3, SENSOR_OUT);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ButtonUI actionButton(BUTTON_UI_PIN);
MotorDriver motors(HBRIDGE_IN1, HBRIDGE_IN2, HBRIDGE_IN3, HBRIDGE_IN4, 26, 25);
Ultrasonic ultrasonic(ECHO_PIN, TRIGG_PIN);
InterfaceUI interfaceUI(display, actionButton, sensorColor, motors, ultrasonic);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Wire.begin();
  sensorColor.begin();
  actionButton.begin();
  motors.begin();
  interfaceUI.begin();
  ultrasonic.begin();

  Serial.println("Starting");

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");

  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  // El motor empieza hacia adelante
  motors.setSpeed(95);
  motors.forward();

}

float anguloAcumulado = 0;
unsigned long tiempoPrevio = 0;


void loop() {
  float gx, gy, gz;

  // Esperar a que haya datos del giroscopio disponibles
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);

    unsigned long tiempoActual = millis();
    float tiempoDelta = (tiempoActual - tiempoPrevio) / 1000.0; // en segundos
    tiempoPrevio = tiempoActual;

    // Asumimos que el giro es sobre el eje Z (en grados por segundo)
    float velocidadAngular = gz; 
    
    // Filtro simple para ignorar el ruido cerca de 0
    if (abs(velocidadAngular) > 0.5) { 
      anguloAcumulado += velocidadAngular * tiempoDelta;
    }

    // Ejecutar giro a la derecha
    motors.right();

    // Detenerse al llegar a 90 grados (-16 por inercia)
    if (abs(anguloAcumulado) >= 74.0) {
      motors.stop();
      delay (1000);
      anguloAcumulado = 0;
      tiempoPrevio = 0;
      motors.forward();
      motors.setSpeed(95);
      delay(10);
    }
  }
}
