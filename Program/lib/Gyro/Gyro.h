#ifndef Gyro_h
#define Gyro_h

#include <Arduino.h>

class Gyro {
    public:
        Gyro();
        void setupMPU();
        void getYawPitchRoll();
        int getYaw();
    private:
        int _yaw;
        int _state;
};

#endif