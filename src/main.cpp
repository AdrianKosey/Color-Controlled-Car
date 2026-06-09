#include <Arduino.h>
#include "InterfaceUI/InterfaceUI.h"
#include "InterfaceUI/ButtonUI.h"
#include "color/TCS230.h"
#include "config/default.h"
#include "motor/MotorDriver.h"
#include "ultrasonic/Ultrasonic.h"

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

  // if (!IMU.begin())
  // {
  //   Serial.println("Failed to initialize IMU!");

  // }

  // Serial.print("Gyroscope sample rate = ");
  // Serial.print(IMU.gyroscopeSampleRate());
  // Serial.println(" Hz");
  // Serial.println();
  // Serial.println("Gyroscope in degrees/second");
  // Serial.println("X\tY\tZ");

  // El motor empieza hacia adelante
  motors.setSpeed(95);
  motors.forward();

}

void loop() {


  motors.forward();
  delay(500);


  motors.stop();
  delay(1000);

  motors.rotateDegreesCW(90);
  delay(1000);


  motors.stop();
  delay(1000);

  motors.forward();
  delay(1000);


  motors.stop();
  delay(1000);

  motors.rotateDegreesCW(120);
  delay(1000);


  motors.stop();
  delay(1000);

  motors.forward();
  delay(500);


  motors.stop();
  delay(1000);

  motors.rotateDegreesCW(120);
  delay(1000);

  motors.stop();
  delay(1000);

  motors.stop();
  delay(1000);

  motors.forward();
  delay(1000);

  motors.stop();
  delay(1000);

  motors.rotateDegreesCCW(180);
  delay(1000);

  motors.stop();
  delay(1000);

  motors.forward();
  delay(1000);


  motors.stop();
  delay(1000);

  motors.rotateDegreesCW(135);
  delay(1000);  

  motors.forward();
  delay(500);

  motors.stop();
  delay(1000);

  motors.rotateDegreesCW(45);
  delay(1000);
  
}