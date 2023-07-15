#include <Arduino.h>
#include <M5Stack.h>
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
#include "Display.h"

#define LGFX_AUTODETECT
#define LGFX_USE_V1

static LGFX lcd;
static LGFX_Sprite canvas(&lcd);

Display::Display() {

}


void Display::setupDisplay(){
	lcd.init();
	lcd.setRotation(1);
	canvas.setColorDepth(8);
	canvas.setTextWrap(false);
	canvas.setTextSize(0.5);
	canvas.createSprite(lcd.width(), lcd.height());
    canvas.setTextColor(BLACK);
    canvas.setFont(&fonts::Font4);
}

void Display::drawUI(int heading,int IR){
    extern int battery;
    extern int Gyrostate;
    extern int IRstate;
    extern int selectID;
    extern int Camerastate;
    extern int GrovalCam;
    canvas.fillScreen(BLACK);
    canvas.drawJpgFile(SD, "/M5UI.jpg");


    if(battery > 24)
        canvas.fillRect(31,23,50*battery/100,20,GREEN);
    else
        canvas.fillRect(31,23,50*battery/100,20,RED);
    canvas.setCursor(87,25);
    canvas.setTextSize(0.7);
    canvas.printf("%d%%",battery);
    canvas.setTextSize(0.5);


    switch(selectID)
    {
        case 0:
            canvas.drawRect(153, 13, 155, 44, GREEN);
            break;
        case 1:
            canvas.drawRect(14, 74, 120, 31, GREEN);
            break;
        case 2:
            canvas.drawRect(14, 115, 120, 31, GREEN);
            break;
        case 3:
            canvas.drawRect(14, 157, 120, 31, GREEN);
            break;
        case 4:
            canvas.drawRect(14, 198, 120, 31, GREEN);
            break;
    }
    if(Gyrostate == 0)
        canvas.fillCircle(110, 90, 8, GREEN);
    else if(Gyrostate == 1)
        canvas.fillCircle(110, 90, 8, RED);
    else if(Gyrostate == 2)
    {
        canvas.setCursor(102,85);
        canvas.printf("OFF");
    }
    if(IRstate == 0)
        canvas.fillCircle(110, 131, 8, GREEN);
    else if(IRstate == 1)
        canvas.fillCircle(110, 131, 8, RED);
    else if(IRstate == 2)
    {
        canvas.setCursor(102,126);
        canvas.printf("OFF");
    }
    if(Camerastate == 0)
        canvas.fillCircle(110, 173, 8, GREEN);
    else if(Camerastate == 1)
        canvas.fillCircle(110, 173, 8, RED);
    else if(Camerastate == 2)
    {
        canvas.setCursor(102,168);
        canvas.printf("OFF");
    }


    canvas.drawCircle(231,152,70,BLACK);
    float _deg = (heading+GrovalCam-90) / (180 / PI);    //mainでheading-GrovalCamしてるので
    float _deg2 = (IR-90) / (180 / PI);
    float _deg3 = (GrovalCam-90) / (180 / PI);
    canvas.fillCircle(231 + 70 * cos(_deg), 152 + 70 * sin(_deg), 7, GREEN);
    canvas.fillCircle(231 + 70 * cos(_deg2), 152 + 70 * sin(_deg2), 7, RED);
    canvas.fillCircle(231 + 70 * cos(_deg3), 152 + 70 * sin(_deg3), 7, BLUE);
    canvas.setCursor(210,137);
    canvas.printf("Gyro:%d",heading);
    canvas.setCursor(205,155);
    canvas.printf("Ball:%d",IR);
    canvas.setCursor(205,173);
    canvas.printf("Goal:%d",GrovalCam);
    canvas.pushSprite(0,0);
}

void Display::drawUIdoMain()
{
    canvas.fillScreen(BLACK);
    canvas.drawJpgFile(SD, "/M5UIdomain.jpg");
    canvas.drawRect(153, 13, 155, 44, GREEN);
    canvas.pushSprite(0,0);
}

void Display::selectButton(){
    extern int selectID;
    extern int doMain;
    extern int Gyrostate;
    extern int IRstate;
    extern int Camerastate;
    extern int Linestate;
    extern int SPEED;
    int speed[5] = {0, 100, 150, 200, 255};
    int speedIndex = 4;
    if(doMain)
    {
        if(M5.BtnB.wasPressed())
        {
            doMain = false;
        }
        return;
    }
    if(M5.BtnC.wasPressed()){
        selectID++;
        if(selectID > 4)
            selectID = 0;
    }
    if(M5.BtnA.wasPressed())
    {
        selectID--;
        if(selectID < 0)
            selectID = 4;
    }
    // if(M5.BtnA.wasPressed() && selectID == 0)
    // {
    //     if(speedIndex < 0)
    //         speedIndex = 4;
    //     SPEED = speed[--speedIndex];
    // }
    if(M5.BtnB.wasPressed())
    {
        switch(selectID)
        {
            case 0:
                doMain = !doMain;
                break;
            case 1:
                if(Gyrostate == 0)
                    Gyrostate = 2;
                else if(Gyrostate == 2)
                    Gyrostate = 0;
                break;
            case 2:
                if(IRstate == 0)
                    IRstate = 2;
                else if(IRstate == 2)
                    IRstate = 0;
                break;
            case 3:
                if(Camerastate == 0)
                    Camerastate = 2;
                else if(Camerastate == 2)
                    Camerastate = 0;
                break;
            case 4:
                if(Linestate == 0)
                    Linestate = 2;
                else if(Linestate == 2)
                    Linestate = 0;
                break;
        }
    }
}

void Display::printval(int val){
    canvas.fillScreen(BLACK);
    canvas.setCursor(140,120);
    canvas.printf("%d",val);
    canvas.pushSprite(0,0);
}

void Display::drawCircle(int degree){
    canvas.fillScreen(BLACK);
    float deg = (degree-90) / (180 / PI);
    canvas.drawCircle(160, 120, 80, WHITE);
    canvas.fillCircle(160 + 80 * cos(deg), 120 + 80 * sin(deg), 10, GREEN);
    canvas.setCursor(160, 0);
    canvas.printf("    ");
    canvas.printf("%4d", degree);
    canvas.pushSprite(0,0);
}

void Display::drawCircle(int degree,int degree2){
    canvas.fillScreen(BLACK);
    float deg = (degree-90) / (180 / PI);
    float deg2 = (degree2-90) / (180 / PI);
    canvas.drawCircle(160, 120, 80, WHITE);
    canvas.fillCircle(160 + 80 * cos(deg), 120 + 80 * sin(deg), 7, GREEN);
    canvas.setCursor(200, 0);
    canvas.printf("Gyro:%4d", degree);
    canvas.fillCircle(160 + 80 * cos(deg2), 120 + 80 * sin(deg2), 7, RED);
    canvas.setCursor(200, 20);
    canvas.printf("Ball:%4d", degree2);
    canvas.pushSprite(0,0);
}