#include "Motor.h"
#include <Arduino.h>

#define PLUS 30
#define SPEED 255

CytronMD motor1(PWM_PWM,16,17);
CytronMD motor2(PWM_PWM,2,5);
CytronMD motor3(PWM_PWM,26,25);
CytronMD motor4(PWM_PWM,1,3);

Motor::Motor() {

}

void Motor::moveTo(int degree) {
  int plus_power = 0;
  int diff = 10;
  extern int heading;

  if(heading >= 40)
  {
    turnL(200 * heading/180);
    return;
  }
  else if(heading <= -40)
  {
    turnR(200 * -heading/180);
    return;
  }


  if (heading >= 0 && heading <= diff)
  {
    plus_power = 0;//-GrovalCam;
  }
  else if (heading <= 0 && heading >= -diff)
  {
    plus_power = 0;//-GrovalCam;
  }
  else if(heading > 0)
  {
    plus_power = -PLUS;
  }
  else
  {
    plus_power = PLUS;
  }
  double motor_power[4];
  double max_power;
  degree -= 90;
  motor_power[0] = cos((45 - degree) / 180.0 * PI);
  motor_power[1] = cos((135 - degree) / 180.0 * PI);
  motor_power[2] = cos((-135 - degree) / 180.0 * PI);
  motor_power[3] = cos((-45 - degree) / 180.0 * PI);

  for (int i = 0; i < 4; i++)
  {
    if (abs(motor_power[i]) > max_power)
    {
      max_power = abs(motor_power[i]);
    }
  }

  for (int i = 0; i < 4; i++)
  {
    motor_power[i] = SPEED * motor_power[i] / max_power + plus_power;
    // for (int j = (num - 1); j > 0; j--)
    // {
    //   ave_motor_power[i][j] = ave_motor_power[i][j - 1];
    // }
    // ave_motor_power[i][0] = motor_power[i];
    // ave_mpPlus = 0;
    // for (int k = 0; k < num; k++)
    // {
    //   ave_mpPlus = ave_mpPlus + ave_motor_power[i][k];
    // }
    // motor_power[i] = ave_mpPlus / num;
  }

  motor1.setSpeed((int)motor_power[0]);
  motor2.setSpeed((int)motor_power[1]);
  motor3.setSpeed((int)motor_power[2]);
  motor4.setSpeed((int)motor_power[3]);
  delay(1);
}

void Motor::stop(){
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    motor3.setSpeed(0);
    motor4.setSpeed(0);
}

void Motor::turnR(int power){
    motor1.setSpeed(power);
    motor2.setSpeed(power);
    motor3.setSpeed(power);
    motor4.setSpeed(power);
}

void Motor::turnL(int power){
    motor1.setSpeed(-power);
    motor2.setSpeed(-power);
    motor3.setSpeed(-power);
    motor4.setSpeed(-power);
}