#ifndef IR_h
#define IR_h

#include <Arduino.h>

class IR {
    public:
        IR();
        int GetIRval(int f);
        int GetIRdir(int i);
        int aveIR();
    private:
        int _angle;
        int _prevIR, _count;
        int _dirIR;
        int _state;
};

#endif