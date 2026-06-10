#include <Arduino.h>
#include "InterfaceUI/InterfaceUI.h"
#include "color/TCS230.h"
#include "config/default.h"
#include "motor/MotorDriver.h"
#include "ultrasonic/Ultrasonic.h"
// OLED
TCS230 sensorColor(S0, S1, S2, S3, SENSOR_OUT);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MotorDriver motors(HBRIDGE_IN1, HBRIDGE_IN2, HBRIDGE_IN3, HBRIDGE_IN4, 26, 25);
Ultrasonic ultrasonic(ECHO_PIN, TRIGG_PIN);
InterfaceUI interfaceUI(display,sensorColor, motors, ultrasonic);


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Wire.begin();
  sensorColor.begin();
  motors.begin();
  ultrasonic.begin();

  Serial.println("Starting");
}

void loop()
{
  interfaceUI.update();  
}