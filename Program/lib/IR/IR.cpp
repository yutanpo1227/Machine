#include <Arduino.h>
#include <Wire.h>
#include "IR.h"


IR::IR()
{
  _angle = -1;
  _dirIR = 0;
  _prevIR = 0;
  _count = 0;
}

int IR::GetIRval(int f) {
  byte _val = 0;
  Wire.beginTransmission(0x0E);
  Wire.write(f);
  Wire.endTransmission();
  Wire.requestFrom(0x0E, 1);
  while (Wire.available()) {
    _val = Wire.read();
  }
  return (int)_val;
}

int IR::GetIRdir(int i) {
  int _a = GetIRval(0x04);
  int _b = GetIRval(0x05);
  int _c = GetIRval(0x06);
  int _d = GetIRval(0x07);
  int _re_angle;
  int _re_strength;

  if (_d < 10) {
    _re_angle = _a;
    _re_strength = _b;
  }
  else {
    _re_angle = _c;
    _re_strength = _d;
  }

  if (i != 1) {
    return _re_strength;
  }
  else {
    return _re_angle * 5;
  }
}

int IR::aveIR() {
  extern int IRstate;
  int _a = 0;
  int _num = 3;
  for (int i = 0; i < _num; i++) {
    _dirIR = GetIRdir(1);
    if (abs(_prevIR - _dirIR) > 70) {
      _count++;
      if (_count == 2) {
        _count = 0;
        _prevIR = _dirIR;
      }
      else {
        _dirIR = _prevIR;
      }
    }
    else {
      _count = 0;
      _prevIR = _dirIR;
    }
    _a += _dirIR;
  }
  _angle = _a / _num;
  if(_angle == -1 || _angle > 1000)
  {
    IRstate = 1;
  }
  if(IRstate == 2 || IRstate == 1)
    return 0;
  return _angle;
}