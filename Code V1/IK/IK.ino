#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//#include "conio.h"
#include "ctype.h"
#include "Ramp.h"
#include "Vector.h"
#include "Servo.h"
//#include <windows.h>
using namespace std;

#define arm1 120
#define arm2 135
//const double PI =  3.1415926;  //It already exists a constant with Arduino.h
#define spacer printf("---------------------------\n")


class point{
  public:
    float ang1;
    float ang2;
    //float ang3;
    //float ang4;
    //float anginf;
    float x;
    float y;

  //functions
    void returnposDK();
    void returnposIK();
    void DK();
    void IK();
};

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

void mode2(Vector<class point>&points);
void print_points(Vector<class point> points);

Servo servoinf, servosup, servosupext, servogir, servogirbase;

void setup(){
  Serial.begin(115200);
  delay(1000);
  servogir.attach(22);
  servosup.attach(25);
  servoinf.attach(23);
  servosupext.attach(26);
  servogirbase.attach(30);
  Serial.println("hello");
  delay(1000);
  programm();
}

void loop(){
  
}

void programm(){
  Serial.println("Welcome to Arm IV Control Program!");
  delay(10);
  Serial.print("This program will help you control your RoboticArm Movements.\n");
  
  Vector <class point> points;
  char mode;
  do{

    do{
      Serial.println("");
      spacer;
      Serial.print("Select the mode:\n1 - Point from angles\n2 - Angles from end point\n3 - Operation\n4 - End program\n");
      mode = Serial.parseInt();
    }while(mode<'1' || mode>'4');

    //FIRST LOOP//
    //..........//
    switch (mode)
    {
      
      case '1':
        point p1;
        p1.returnposDK();
        //spacer;
        //char add;
        // do{
        //   puts("Do you want to add the point to the path?  Y / N");
        //   add = Serial.parseInt();
        //   add = tolower(add);
        // }while(add != 'y' && add != 'n');
        // if(add == 'y'){
        //   points.push_back(p1);
        // }
        spacer;
        break;
      
      //-----------------------------------------------//
      case '2':
        //Getting initial angles for each joints
        point p2;
        p2.returnposIK();
        // spacer;
        // do{
        //   puts("Do you want to add the point to the path?  Y / N");
        //   add = Serial.parseInt();
        //   add = tolower(add);
        // }while(add != 'y' && add != 'n');
        // if(add == 'y'){
        //   points.push_back(p2);
        // }
        spacer;
        break;

      //--------------------------------------------------//
      //SECOND LOOP//
      case '3':
        mode2(points);
        break;
      
    }
  }while(mode!='4');
  return 0;
}


void point::returnposDK(){
  float max_range = arm1 + arm2;
  float min_range = 100;
  bool posible = true;
  Serial.print("\nIntroduce the angles of each joint\n");
  do{
    Serial.print("ang1 (0 - 180): ");
    ang1 = Serial.parseInt();
  }while(ang1 < 0 || ang1 > 180);
  do{
    Serial.print("ang2 (-40 - 140): ");
    ang2 = Serial.parseInt();
  }while(ang2 < -40 || ang2 > 140);
  Serial.println("Correct angles introduced");
  spacer;  
  DK();
  Serial.print("Point selected: ("+String(x,1)+" / "+String(y,1)+")\n");

}

void point::DK(){
  float aux1, aux2;
  aux1 = ang1 * PI/180;
  aux2 = ang2 * PI/180;
  x = (arm1*cos(aux1))+(arm2*cos(aux1 - aux2));
  y = (arm1*sin(aux1))+(arm2*sin(aux1 - aux2));
}

void point::returnposIK(){

  float max_range = arm1 + arm2;
  float min_range = 100;
  bool posible = true;
  Serial.print("\nIntroduce the coords of the point you want to reach\n");
  do{
    do{
      Serial.print("x coord (0 - 255): ");
      x=Serial.parseInt();
    }while(x < 0 || x > 255);
    do{
      Serial.print("y coord (60 - 255): ");
      y=Serial.parseInt();
    }while(y < 0 || y > 255);

    float d = sqrt(x*x + y*y);
    if(d<=max_range && d>=min_range){
      Serial.println("Point available in the workspace");
      posible=false;
    }else{
      Serial.println("Point out of the workspace, try again\n");
    }
  }while(posible);
  IK();
  Serial.print("The angles are ("+String(ang1,2)+" , "+String(ang2,2)+"\n");

}

void point::IK(){
  ang2 = acos(((x*x + y*y) - (pow(arm1,2)) - (pow(arm2,2)))/(2*arm1*arm2))*180/PI;
  ang1 = (atan(y/x) - atan((arm2*(sin(-ang2*PI/180))) / (arm1 + (arm2*(cos(-ang2*PI/180))))))*180/PI;
}

void mode2(Vector <class point> &points){
  char mode2;
  do{
    do{
      Serial.println("");
      spacer;
      Serial.print("Select the operation:\n1 - Insert points\n2 - Remove points\n3 - Move to point\n4 - Path with points\n5 --> BACK\n");
      mode2 = Serial.parseInt();
    }while(mode2<'1' || mode2>'5');
  

    switch (mode2){
      case '1':
        int entry;
        Serial.println("");
        spacer;
        do{Serial.println("Number of points?");entry=Serial.parseInt();}while(entry <= 0);
        for(int i=0; i<entry; i++){
          point p3;
          spacer;
          p3.returnposIK();
          points.push_back(p3);
        }
        break;
      
      case '2':
        Serial.println("");
        spacer;
        if(points.size()==1 || points.size()==0){
          Serial.print("List is empty");
          points.clear();
          break;
        }
        do{Serial.print("Which point do you want to remove? ("+String(1-points.size())+")\n");print_points(points);entry=Serial.parseInt();}while(entry < 0 || entry > points.size());
          points.remove(entry);
        
        break;
        
      case '3':
        while(true){
          Serial.println("");
          spacer;
          do{
            Serial.print("Which point do you want to move to? ("+String(1-points.size())+")\n");print_points(points);entry=Serial.parseInt();}while(entry < 0 || entry > points.size());
          //move to robot to point p1
          //
          //
          //
          //
          char add;
          do{
            Serial.println("Do you want to move to other point?  Y / N");
            add = Serial.parseInt();
            add = tolower(add);
          }while(add != 'y' && add != 'n');
          if(add == 'n'){
            break;
          }
        }
        break;
      
      case '4':
        while(true){
          spacer;
          //mode3
          //
          //
          //
        }
      /*
      case 'P':
        puts("ok");
        break;*/
    }
  }while(mode2!='5');
}

void print_points(Vector <class point> points){
  for(int i=0; i < points.size(); i++){
    spacer;
    Serial.print("\nPoint "+String(i+1)+":");
    Serial.print("\n- ( "+String(points[i].x,2)+" , "+String(points[i].y,2)+" ) pos\n- ( "+String(points[i].ang1,2)+" , "+String(points[i].ang2,2)+" ) ang\n");
  }
}