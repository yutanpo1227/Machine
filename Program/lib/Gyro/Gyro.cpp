#include "Gyro.h"
#include <Arduino.h>
#include <MPU6050_6Axis_MotionApps20.h>

MPU6050 mpu;
// MPU control/status vars
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

float ypr[3];           // [roll, pitch, yaw]   roll/pitch/yaw container and gravity vector

Gyro::Gyro()
{
}

void Gyro::setupMPU()
{
  extern int Gyrostate;
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  mpu.initialize();
  devStatus = mpu.dmpInitialize();                                                                                                                                                                                                                                                                                                                                                    

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXAccelOffset(-2634);
  mpu.setYAccelOffset(-38);
  mpu.setZAccelOffset(472);
  mpu.setXGyroOffset(203);
  mpu.setYGyroOffset(54);
  mpu.setZGyroOffset(28);

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
		mpu.CalibrateAccel(6);
		mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Gyrostate = 1;
    Serial.print("DMP Initialization failed.");
  }
}

void Gyro::getYawPitchRoll()
{
  extern int Gyrostate;
  if(Gyrostate == 2)
    return;
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  }
  delay(1);
}

int Gyro::getYaw()
{
  extern int Gyrostate;
  if(Gyrostate == 2)
    return 0;
  getYawPitchRoll();
  _yaw = int(ypr[0] * 180 / M_PI);
  return _yaw;
}