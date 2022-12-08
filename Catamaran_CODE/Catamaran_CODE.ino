/*
  This has been coded by Berk Yozkan to run simple tasks over a basic conveyor belt.
  To find out more about the project check the github page: https://github.com/berkyoskan/Autonomous-Catamaran
  This code is in the public domain.
*/
#include <RPLidar.h> //Adding Libraries to Project
#include <Servo.h>

Servo lmotor; //Defining ESC's as servos to be able to generate required PWM signal.
Servo rmotor;
RPLidar lidar; //Defining lidar object to be used with LIDAR library.

#define RPLIDAR_MOTOR 5 //Defining the pin of the RPLIDAR's motor as D5

int barrier = 850; //The distance between LIDAR and front will be this value.
int clearence = 800; //The distance between LIDAR and sides will be this value.
float frontdistance, backdistance, rightdistance, leftdistance; //Defining variables for each direction.

void setup() {
  lidar.begin(Serial); //Starting LIDAR
  pinMode(RPLIDAR_MOTOR, OUTPUT); //Defining LIDAR motor pin as output.
  lmotor.attach(2); //Attaching Left Motor ESC to D2
  rmotor.attach(3); //Attaching Right Motor ESC to D3
  lmotor.writeMicroseconds(1500); //Generating signal to enable left motor ESC
  rmotor.writeMicroseconds(1500); //Generating signal to enable right motor ESC
  delay(5000);  //Delay before whole code starts
}

void loop() {
  if (IS_OK(lidar.waitPoint())) { //Cheking LIDAR's availability
    float angle = lidar.getCurrentPoint().angle; //Capturing the angle data from LIDAR
    if (lidar.getCurrentPoint().quality > 10) { //Cheking the captured data's quality, it should be above 10%
      if (angle < 185 && angle > 175) { //If the captured angle is between 185 and 175 degrees it gets triggered.
        frontdistance = lidar.getCurrentPoint().distance; //Capturing the distance data on this specific angle.
      }
      if (angle < 95 && angle > 85) { //If the captured angle is between 95 and 85 degrees it gets triggered.
        rightdistance =  lidar.getCurrentPoint().distance; //Capturing the distance data on this specific angle.
      }
      if (angle < 275 && angle > 265) { //If the captured angle is between 275 and 265 degrees it gets triggered.
        leftdistance =  lidar.getCurrentPoint().distance; //Capturing the distance data on this specific angle.
      }
    }
    if ((frontdistance < barrier && frontdistance > 0 && leftdistance < rightdistance && leftdistance < clearence && rightdistance > 0 && leftdistance > 0) || (frontdistance < barrier && frontdistance > 0 && leftdistance >= clearence && rightdistance >= clearence)) {
      //If the there is an obstacle in front compares both sides and if the left side is more available than right activates right motor to turn left.
      lmotor.writeMicroseconds(1500);
      rmotor.writeMicroseconds(1600);
    }
    else if (frontdistance < barrier && frontdistance > 0 && leftdistance > rightdistance && rightdistance < clearence && rightdistance > 0 && leftdistance > 0) {
      //If the there is an obstacle in front compares both sides and if the right side is more available than left activates left motor to turn right.
      lmotor.writeMicroseconds(1600);
      rmotor.writeMicroseconds(1500);
    }
    else if (frontdistance > barrier || (leftdistance >= clearence && rightdistance >= clearence)) {
      //If there is no obstacle in front goes forward
      lmotor.writeMicroseconds(1600);
      rmotor.writeMicroseconds(1600);
    }
  }
  else {
    //If the LIDAR is not available tries to activate
    analogWrite(RPLIDAR_MOTOR, 0);
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 255);// Sets LIDAR's motor to specific speed.
      delay(1000);
    }
  }
}
