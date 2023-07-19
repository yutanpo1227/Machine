#include <Arduino.h>
#include <M5Stack.h>
#include <SoftwareSerial.h>
#include "Camera.h"
#include "Gyro.h"
#include "Motor.h"
#include "Display.h"
#include "IR.h"
#include "Line.h"


Gyro gyro = Gyro();
Motor motor = Motor();
IR ir = IR();
Display display = Display();
Camera camera = Camera();
Line line = Line();

int heading = 0;
int battery;
int GrovalCam = 0;
int IRstate = 0;
int Gyrostate = 0;
int Camerastate = 0;
int Linestate = 0;
int selectID = 0;

int losscount = 0;

bool doMain = false;

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  //Serial2.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 36, 0);
  gyro.setupMPU();
  display.setupDisplay();
}

void loop() {
  M5.update();
  int _irAngle = ir.aveIR();
  camera.Read();
  heading = gyro.getYaw() - GrovalCam;
  battery = M5.Power.getBatteryLevel();
  display.selectButton();

  if(doMain == false)
  {
    display.drawUI(heading, _irAngle);
    motor.stop();
  }
  else
  {
    display.drawUIdoMain();
    if(_irAngle == 0 || _irAngle == 5 || _irAngle == 355)
    {
      motor.moveTo(0);
    }
    else
    {
      if(_irAngle >= 10 && _irAngle <= 180)
      {
        motor.moveTo(_irAngle + 90*_irAngle/180);
      }
      else if(_irAngle > 180 && _irAngle <= 350)
      {
        motor.moveTo(_irAngle - 90*(360-_irAngle)/180);
      }
    }
  }
}