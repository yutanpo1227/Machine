#include <Arduino.h>
#include <M5Stack.h>
#include <SoftwareSerial.h>
#include "Camera.h"
#include "Display.h"


Camera::Camera()
{
}

void Camera::setupCamera()
{

}

void Camera::Read()
{
    extern int GrovalCam;
    extern int Camerastate;
    extern int losscount;
    if(Camerastate == 2)
    {
        GrovalCam = 0;
        return;
    }
    losscount++;
    if(losscount > 10)
    {
        Camerastate = 1;
    }
    while(Serial2.available())
    {
        Camerastate = 0;
        losscount = 0;
        String val = Serial2.readStringUntil('\n');
        GrovalCam = -(val.toInt() - 55);
    }
}