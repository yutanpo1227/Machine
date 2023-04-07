#ifndef IR_h
#define IR_h

#include <Arduino.h>

class IR {
    public:
        IR();
        int GetIRval(int f);
        int GetIRdir(int i);
        int aveIR();
        int angle;
        int prevIR, count;
        int dirIR;
        int state;
    private:
};

#endif