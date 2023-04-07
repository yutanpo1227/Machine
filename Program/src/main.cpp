#include <Arduino.h>
#include <M5Stack.h>
#include <SoftwareSerial.h>
#include "Camera.h"
#include "Gyro.h"
#include "Motor.h"
#include "Display.h"
#include "IR.h"


Gyro gyro = Gyro();
Motor motor = Motor();
IR ir = IR();
Display display = Display();
Camera camera = Camera();

int heading=0;
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
  heading = gyro.getYaw();
  int IR = ir.aveIR();
  camera.Read();
  battery = M5.Power.getBatteryLevel();
  display.selectButton();

  if(doMain == false)
  {
    display.drawUI(heading, IR);
    motor.stop();
  }
  else
  {
    display.drawUIdoMain();
    if(IR == 0 || IR == 5 || IR == 355)
    {
      motor.moveTo(0);
    }
    else
    {
      if(IR >= 10 && IR <= 180)
      {
        motor.moveTo(IR + 70*IR/180);
      }
      else if(IR > 180 && IR <= 350)
      {
        motor.moveTo(IR - 70*(360-IR)/180);
      }
    }
  }
}