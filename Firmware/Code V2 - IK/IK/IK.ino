#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//#include "conio.h"
#include "ctype.h"
#include "Ramp.h"
//#include "Vector.h"
#include "Servo.h"
//#include <windows.h>
using namespace std;

#define arm1 120
#define arm2 135
//const double PI =  3.1415926;  //It already exists a constant with Arduino.h
#define spacer printf("---------------------------\n")
unsigned long currentMillis;
unsigned long previousMillis;
long previousStepMillis = 0;

Servo servoinf, servosup, servosupext, servogir, servogirbase;

float xTarget;
float yTarget;
float ang1;
float ang2;
int stepFlag = 0;
float x;
float y;

class Interpolation{
  public:
    rampInt myRamp;
    int interpolationFlag = 0;
    int prevValue;

    int go(int input, int duration){
      char16_t type_mov;
      if (input != prevValue){
        interpolationFlag = 0;  //we activate the code and let the interpolation work
      }
      
      prevValue = input;
      if(interpolationFlag == 0){  //with the object myramp we created we execute go ramp own function
        myRamp.go(input,duration,LINEAR, ONCEFORWARD);
        interpolationFlag = 1;
      }

      int output = myRamp.update();
      return output;  //we return the value of next pos for one axis
    }
};

Interpolation interpX;
Interpolation interpY; //we create an interpolation object for each axis, it will return us each pos for each axis


void setup(){
  
  servogir.attach(22);   //retorcido
  servosup.attach(25);   //medio
  servoinf.attach(23);   //goma
  servosupext.attach(26);  //extremo
  servogirbase.attach(30);

  Serial.begin(9600);

  xTarget = 100;
  yTarget = 100;
}

void loop(){
  
  currentMillis = millis();

  if (currentMillis - previousMillis >= 10){
    previousMillis = currentMillis;

    Serial.print(x);
    Serial.print("  ");
    Serial.print(y);
    Serial.print("  ");
    Serial.print(ang1);
    Serial.print("  ");
    Serial.println(ang2);

    if (stepFlag == 0 && currentMillis-previousStepMillis >= 2500){
      xTarget = 0;
      yTarget = 100;
      previousStepMillis = currentMillis;
      stepFlag = 1;
    }
    else if (stepFlag == 1 && currentMillis-previousStepMillis >= 2500){
      xTarget = 0;
      yTarget = 230;
      previousStepMillis = currentMillis;
      stepFlag = 2;
    }
    else if (stepFlag == 2 && currentMillis-previousStepMillis >= 2000){
      xTarget = 100;
      yTarget = 200;
      previousStepMillis = currentMillis;
      stepFlag = 3;
    }  
    else if (stepFlag == 3 && currentMillis-previousStepMillis >= 2000){
      xTarget = 100;
      yTarget = 100;
      previousStepMillis = currentMillis;
      stepFlag = 0;
    }  

    

  
    //interpolation//
    y = interpY.go(yTarget+10,700);
    x = interpX.go(xTarget, 700);   //we introduce the final point and the duration and we obtain the next point we need to move to

    ang2 = acos(((x*x + y*y) - (pow(arm1,2)) - (pow(arm2,2)))/(2*arm1*arm2))*180/PI;
    ang1 = (atan(y/x) - atan((arm2*(sin(-ang2*PI/180))) / (arm1 + (arm2*(cos(-ang2*PI/180))))))*180/PI;
    
    servoinf.write(ang1);
    servosup.write(ang2+60);

  }
  
  
} 

void DK(){
  float aux1, aux2;
  aux1 = ang1 * PI/180;
  aux2 = ang2 * PI/180;
  int x = (arm1*cos(aux1))+(arm2*cos(aux1 - aux2));
  int y = (arm1*sin(aux1))+(arm2*sin(aux1 - aux2));
}






