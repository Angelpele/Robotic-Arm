#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"
#include "ctype.h"
//#include "Ramp.h"
#include <vector>
#include "Servo.h"
#include <windows.h>



class Interpolation{
  public:
    rampInt myRamp;
    int interpolationSwitch = 0;
    int prevValue;

    int go(int input, int duration, int movement){
      char16_t type_mov;
      if (input != prevValue){
        interpolationSwitch = 0;  //we activate the code and let the interpolation work
      }
      switch (movement){
        case 1: type_mov = LINEAR; break;
        case 2: type_mov = QUADRATIC_INOUT; break;
        case 3: type_mov = SINUSOIDAL_IN; break;
        case 4: type_mov = EXPONENTIAL_IN; break;
        case 5: type_mov = ELASTIC_IN; break;
      }
      if(interpolationSwitch == 0){  //with the object myramp we created we execute go ramp own function
        myRamp.go(input,duration,type_mov, ONCEFORWARD);
        interpolationSwitch = 1;
      }

      int output = myRamp.update();
      return output;  //we return the value of next pos for one axis
    }
};

Interpolation interpX;
Interpolation interpY; //we create an interpolation object for each axis, it will return us each pos for each axis

Servo servoinf, servosup, servosupext, servogir, servogirbase;

void setup(){
  //Serial.begin(9600);
  servogir.attach(22);
  servosup.attach(25);
  servoinf.attach(23);
  servosupext.attach(26);
  servogirbase.attach(30);
}



void loop(){
  servosup.write(20);
}


void moveto(int x, int y){
  
}
