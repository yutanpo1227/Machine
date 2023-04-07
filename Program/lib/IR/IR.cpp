#include <Arduino.h>
#include <Wire.h>
#include "IR.h"


IR::IR()
{
  angle = -1;
  dirIR = 0;
  prevIR = 0;
  count = 0;
  state = 0;
}

int IR::GetIRval(int f) {
  byte val = 0;
  Wire.beginTransmission(0x0E);
  Wire.write(f);
  Wire.endTransmission();
  Wire.requestFrom(0x0E, 1);
  while (Wire.available()) {
    val = Wire.read();
  }
  return (int)val;
}

int IR::GetIRdir(int i) {
  int a = GetIRval(0x04);
  int b = GetIRval(0x05);
  int c = GetIRval(0x06);
  int d = GetIRval(0x07);
  int re_angle;
  int re_strength;

  if (d < 10) {
    re_angle = a;
    re_strength = b;
  }
  else {
    re_angle = c;
    re_strength = d;
  }

  if (i != 1) {
    return re_strength;
  }
  else {
    return re_angle * 5;
  }
}

int IR::aveIR() {
  extern int IRstate;
  int a = 0;
  int num = 3;
  for (int i = 0; i < num; i++) {
    dirIR = GetIRdir(1);
    if (abs(prevIR - dirIR) > 70) {
      count++;
      if (count == 2) {
        count = 0;
        prevIR = dirIR;
      }
      else {
        dirIR = prevIR;
      }
    }
    else {
      count = 0;
      prevIR = dirIR;
    }
    a += dirIR;
  }
  angle = a / num;
  if(angle == -1)
  {
    state = 1;
  }
  if(IRstate == 2)
    return 0;
  return angle;
}