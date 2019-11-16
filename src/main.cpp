#include <Arduino.h>
#include <string.h>
#include <drone.h>
#include <joystick.h>

String ssid = "TELLO-FE32D1";
String password = "";

Drone drone(ssid, password);
Joystick joystickDirection(15, 34, 35); // pins: btn, x, y
Joystick joystickAltitude(4, 32, 33); //pins: btn, x, yy

void setup()
{
  Serial.begin(9600);

  drone.joystickDirection = &joystickDirection;
  drone.joystickAltitude = &joystickAltitude;

  /*for (size_t i = 0; i < 5; i++)
  {
    Serial.println(i);
    delay(1000);
  }
  */

  joystickDirection.addButtonListener(&drone);
  joystickAltitude.addButtonListener(&drone);
  
  drone.connect();
  drone.setIp ("192.168.10.1");

  

  
}

void loop()
{
  joystickDirection.loop();
  joystickAltitude.loop();
  drone.loop();;
} 