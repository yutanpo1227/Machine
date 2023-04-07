#ifndef Gyro_h
#define Gyro_h

#include <Arduino.h>

class Gyro {
    public:
        Gyro();
        void setupMPU();
        void getYawPitchRoll();
        int getYaw();
        int yaw;
        int state;
    private:
};

#endif