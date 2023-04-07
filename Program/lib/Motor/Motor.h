#ifndef Motor_h
#define Motor_h

#include <Arduino.h>
#include <CytronMotorDriver.h>

class Motor {
    public:
        Motor();
        void moveTo(int degree);
        void stop();
        void turnR(int power);
        void turnL(int power);
    private:
};

#endif