#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "System/SystemManager.h"
#include "color/TCS230.h"
#include "config/default.h"
#include "motor/MotorDriver.h"
#include "ultrasonic/Ultrasonic.h"
// OLED
TCS230 sensorColor(S0, S1, S2, S3, SENSOR_OUT);
MotorDriver motors(HBRIDGE_IN1, HBRIDGE_IN2, HBRIDGE_IN3, HBRIDGE_IN4, 26, 25);
Ultrasonic ultrasonic(ECHO_PIN, TRIGG_PIN);
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
SystemManager systemManager(sensorColor, motors, ultrasonic, webSocket);


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  sensorColor.begin();
  motors.begin();
  ultrasonic.begin();
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  MDNS.begin("esp32.local");
  LittleFS.begin(true);
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not found");
  });
  server.on("/", []() {
    File index = LittleFS.open("/index.html", "r");
    if (!index) {
      server.send(500, "text/plain", "Failed to open index.html");
      return;
    }
    server.streamFile(index, "text/html");
    index.close();
  });
  server.on("/style.css", []() {
    File index = LittleFS.open("/style.css", "r");
    if (!index) {
      server.send(500, "text/plain", "Failed to open style.css");
      return;
    }
    server.streamFile(index, "text/css");
    index.close();
  });
  server.on("/script.js", []() {
    File index = LittleFS.open("/script.js", "r");
    if (!index) {
      server.send(500, "text/plain", "Failed to open script.js");
      return;
    }
    server.streamFile(index, "application/javascript");
    index.close();
  });
  server.begin();
  webSocket.begin();
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
  Serial.println("Starting");
}

void loop()
{
  systemManager.update();
  delay(10);
}