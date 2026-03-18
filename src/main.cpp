#include <Arduino.h>
#include "InterfaceUI/InterfaceUI.h"
#include "InterfaceUI/ButtonUI.h"
#include "color/TCS230.h"
#include "config/default.h"
#include "motor/MotorDriver.h"
// OLED
TCS230 sensorColor(S0, S1, S2, S3, SENSOR_OUT);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ButtonUI actionButton(BUTTON_UI_PIN);
MotorDriver motors(HBRIDGE_IN1, HBRIDGE_IN2, HBRIDGE_IN3, HBRIDGE_IN4, 26, 25);
InterfaceUI interfaceUI(display, actionButton, sensorColor, motors);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Wire.begin();
  sensorColor.begin();
  actionButton.begin();
  motors.begin();
  interfaceUI.begin();

  Serial.println("DOme");

}

void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    delayMicroseconds(200);
    Serial.print("R:");
    Serial.print(pulseIn(SENSOR_OUT, LOW));

    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    delayMicroseconds(200);
    Serial.print(" G:");
    Serial.print(pulseIn(SENSOR_OUT, LOW));

    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    delayMicroseconds(200);
    Serial.print(" B:");
    Serial.println(pulseIn(SENSOR_OUT, LOW));

    delay(500);*/
    interfaceUI.update();
    delay(100);

}