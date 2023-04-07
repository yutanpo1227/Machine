#ifndef Display_h
#define Display_h

#include <Arduino.h>

class Display {
    public:
        Display();
        void setupDisplay();
        void drawUI(int heading, int IR);
        void drawUIdoMain();
        void selectButton();
        void printval(int val);
        void drawCircle(int degree);
        void drawCircle(int degree, int degree2);
    private:
};

#endif