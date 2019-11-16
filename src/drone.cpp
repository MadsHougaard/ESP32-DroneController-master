#include <Arduino.h>
#include <drone.h>
#include <joystick.h>
#include <Position.h>

Drone::Drone(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password; 
}

void Drone::connect()
{
    Serial.println("drone begin");
    //Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->ssid.c_str(), this->password.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    if(udp.listen(udpPort)) {
        this->myIp = WiFi.localIP().toString();
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
      
        udp.onPacket(
            [this](AsyncUDPPacket packet) -> void
            {
                // make a string from the data
                String s((char*)packet.data());
                s = s.substring(0, packet.length()); 
                s.trim();
                // send string to method
                this->commandResponse(s);
            }
        );
    }
}

void Drone::sendCommand(String command)
{
    udpSender.beginPacket(this->droneIp.c_str(), udpPort);
    udpSender.printf(command.c_str());
    udpSender.endPacket();    
}

void Drone::setIp(String ip)
{
    this->droneIp = ip;
}
        
void Drone::commandResponse(String response)
{
    Serial.print("got following response: ");
    Serial.println(response.c_str());
    Serial.print("message length: ");
    Serial.println(response.length());
}

void Drone::ButtonTakeoff()
{
    if (flying == false)
    {
        this->flying = true;
        Serial.println("takeoff");
        this->sendCommand("takeoff");
    }
    else
    {
        Serial.println("land");
        this->sendCommand("land");
        this->flying = false;
    }
}

void Drone::ButtonCommand()
{
    if (commandReady == false)
    {
        this->commandReady = true;
        Serial.println("Command Ready");
        this->sendCommand("command");
    }
    else
    {
        Serial.println("awaiting command");
        this->commandReady = false;
    }
}



void Drone::loop()
{

    // Using Position object to retrieve information
    

    Position joystickDirectionPosition = this->joystickDirection->getPosition();
    Position joystickAltitudePosition = this->joystickAltitude->getPosition();

    //Position joystickAltitude = this->joystick->getPosition();
    /*
    if (joystickPosition.x != 0)
    {
        if (joystickPosition.y != 0)
        {
        Serial.print(joystickPosition.x);
        Serial.print(" ");
        Serial.println(joystickPosition.y);
        }
    }
    */
   delay(500);
    Serial.print(this->joystickDirection->getX());
    Serial.print(" ");
    Serial.print(this->joystickDirection->getY());

    Serial.println();
    Serial.print(" ");

    Serial.print(this->joystickAltitude->getX());
    Serial.print(" ");
    Serial.print(this->joystickAltitude->getY());
    


    
    // Using joystick methods
    

    if (joystickDirectionPosition.x > -1 && joystickDirectionPosition.x < 1 && joystickDirectionPosition.y > -1 && joystickDirectionPosition.y < 1 && joystickAltitudePosition.x > -1 && joystickAltitudePosition.x < 1 && joystickAltitudePosition.y > -1 && joystickAltitudePosition.y < 1) 
    {
        //Serial.println("hover");
        this->sendCommand("rc 0 0 0 0");

        // We could have used the "stop" command instead, but it seems too abrupt
    }
    else 
    {

    // Forward
        if (joystickDirectionPosition.x > 500 && joystickDirectionPosition.x < 2000)
        {
            //Serial.println("forward slow");   
            this->sendCommand("rc 0 10 0 0");
        }
        if (joystickDirectionPosition.x > 2000)
        {
            //Serial.println("forward fast");
            this->sendCommand("rc 0 25 0 0");
            
        }

    // Backward
        if (joystickDirectionPosition.x < -500 && joystickDirectionPosition.x > -2000)
        {
            //Serial.prinln("back slow");
            this->sendCommand("rc 0 -10 0 0");
        }
        if (joystickDirectionPosition.x < -2000)
        {
            //Serial.println("back fast");
            this->sendCommand("rc 0 -25 0 0");
        }
    
    // Right
        if (joystickDirectionPosition.y > 500 && joystickDirectionPosition.y < 2000)
        {
            //Serial.println("right slow");
            this->sendCommand("rc 10 0 0 0");
        }
        if (joystickDirectionPosition.y > 2000)
        {
            //Serial.println("right fast");
            this->sendCommand("rc 25 0 0 0");
        }

    // Left
        if (joystickDirectionPosition.y < -500 && joystickDirectionPosition.y > -2000)
        {
            //Serial.println("left slow");
            this->sendCommand("rc -10 0 0 0");
        }
        if (joystickDirectionPosition.y < -2000)
        {
            //Serial.println("left fast");
            this->sendCommand("rc -25 0 0 0");
        }

    // Right-forward
        if (joystickDirectionPosition.x > 500 && joystickDirectionPosition.x < 2000 && joystickDirectionPosition.y > 500 && 
        joystickDirectionPosition.y < 2000)
        {
            //Serial.println("right-forward slow");
            this->sendCommand("rc 10 10 0 0");
        }
        if (joystickDirectionPosition.x > 2000 && joystickDirectionPosition.y > 2000)
        {
            //Serial.println("right-forward fast");
            this->sendCommand("rc 25 25 0 0");
        }
        
    // Left-forward
        if (joystickDirectionPosition.x > 500 && joystickDirectionPosition.x < 2000 && joystickDirectionPosition.y < -500 &&
        joystickDirectionPosition.y > -2000)
        {
            //Serial.println("left-forward slow");
            this->sendCommand("rc -10 10 0 0");
        }
        if (joystickDirectionPosition.x > 2000 && joystickDirectionPosition.y < -2000)
        {
            //Serial.println("left-forward fast");
            this->sendCommand("rc -25 25 0 0");
        }

    // Right-backward
        if (joystickDirectionPosition.x < -500 && joystickDirectionPosition.x > -2000 && joystickDirectionPosition.y > 500 && 
        joystickDirectionPosition.y < 2000)
        {
            //Serial.println("right-backward slow");
            this->sendCommand("rc 10 -10 0 0");
        }
        if (joystickDirectionPosition.x < -2000 && joystickDirectionPosition.y > 2000)
        {
            //Serial.println("right-backward fast");
            this->sendCommand("rc 25 -25 0 0");
        }
        
    // Left-backward
        if (joystickDirectionPosition.x < -500 && joystickDirectionPosition.x > -2000 && joystickDirectionPosition.y < -500 &&
        joystickDirectionPosition.y > -2000)
        {
            //Serial.println("left-backward slow");
            this->sendCommand("rc -10 -10 0 0");
        }
        if (joystickDirectionPosition.x < -2000 && joystickDirectionPosition.y < -2000)
        {
            //Serial.println("left-backward fast");
            this->sendCommand("rc -25 -25 0 0");
        } 

    // Up
        if (joystickAltitudePosition.x > 1000)
      
        {
            //Serial.println("up");
            this->sendCommand("rc 0 0 25 0");
        }        

    // Down
        if (joystickAltitudePosition.x < -1000)
        {
            //Serial.println("down");
            this->sendCommand("rc 0 0 -25 0");
        }

    // Flip left
        if (joystickAltitudePosition.y < -1000)
        {
            //Serial.prinln("flip left");
            this->sendCommand("flip l");
        }
    
    // >Flip right
        if (joystickAltitudePosition.y > 1000)
        {
            //Serial.println("flip right")
            this->sendCommand("flip r");
        }
    }
}
