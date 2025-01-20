#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"
#include "ctype.h"
#include "IK_move.ino"
#include <vector>

using namespace std;

#define arm1 120
#define arm2 135
const double PI =  3.1415926;
#define spacer for(int i=0;i<=40;i++){printf("=");}printf("\n");


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


void mode2(vector<class point>&points);
void print_points(vector<class point> points);

int main(){
  printf("Welcome to Arm IV Control Program!\n");
  printf("This program will help you control your Robotic Arm Movements.\n");
  
  vector <class point> points;
  char mode;
  do{

    do{
      puts("");
      spacer;
      printf("Select the mode:\n1 - Point from angles\n2 - Angles from end point\n3 - Operation\n4 - End program\n");
      mode = getch();
    }while(mode<'1' || mode>'4');

    //FIRST LOOP//
    //..........//
    switch (mode)
    {
      
      case '1':
        point p1;
        p1.returnposDK();
        spacer;
        char add;
        do{
          puts("Do you want to add the point to the path?  Y / N");
          add = getch();
          add = tolower(add);
        }while(add != 'y' && add != 'n');
        if(add == 'y'){
          points.push_back(p1);
        }
        spacer;
        break;
      
      //-----------------------------------------------//
      case '2':
        //Getting initial angles for each joints
        point p2;
        p2.returnposIK();
        spacer;
        do{
          puts("Do you want to add the point to the path?  Y / N");
          add = getch();
          add = tolower(add);
        }while(add != 'y' && add != 'n');
        if(add == 'y'){
          points.push_back(p2);
        }
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
  printf("\nIntroduce the angles of each joint\n");
  do{
    printf("ang1 (0 - 180): ");
    scanf("%f", &ang1);
  }while(ang1 < 0 || ang1 > 180);
  do{
    printf("ang2 (-40 - 140): ");
    scanf("%f", &ang2);
  }while(ang2 < -40 || ang2 > 140);
  puts("Correct angles introduced");
  spacer;  
  DK();
  printf("Point selected: (%.1f / %.1f)\n", x, y);

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
  printf("\nIntroduce the coords of the point you want to reach\n");
  do{
    do{
      printf("x coord (0 - 255): ");
      scanf("%f", &x);
    }while(x < 0 || x > 255);
    do{
      printf("y coord (60 - 255): ");
      scanf("%f", &y);
    }while(y < 0 || y > 255);

    float d = sqrt(x*x + y*y);
    if(d<=max_range && d>=min_range){
      puts("Point available in the workspace");
      posible=false;
    }else{
      puts("Point out of the workspace, try again\n");
    }
  }while(posible);
  IK();
  printf("The angles are (%.2f , %.2f)\n", ang1, ang2);

}

void point::IK(){
  ang2 = acos(((x*x + y*y) - (pow(arm1,2)) - (pow(arm2,2)))/(2*arm1*arm2))*180/PI;
  ang1 = (atan(y/x) - atan((arm2*(sin(-ang2*PI/180))) / (arm1 + (arm2*(cos(-ang2*PI/180))))))*180/PI;
}

void mode2(vector <class point> &points){
  char mode2;
  do{
    do{
      puts("");
      spacer;
      printf("Select the operation:\n1 - Insert points\n2 - Remove points\n3 - Move to point\n4 - Path with points\n5 --> BACK\n");
      mode2 = getch();
    }while(mode2<'1' || mode2>'5');
  

    switch (mode2){
      case '1':
        int entry;
        puts("");
        spacer;
        do{puts("Number of points?");scanf("%i", &entry);}while(entry <= 0);
        for(int i=0; i<entry; i++){
          point p3;
          spacer;
          p3.returnposIK();
          points.push_back(p3);
        }
        break;
      
      case '2':
        puts("");
        spacer;
        if(points.size()==0){
          printf("List is empty");
          points.clear();
          break;
        }
        else if(points.size()==1){
          print_points(points);
          points.clear();
          break;
        }
        do{printf("Which point do you want to remove? (1-%i)\n", points.size());print_points(points);scanf("%i", &entry);}while(entry < 0 || entry > points.size());
          points.erase(points.begin() + entry);
        
        break;
        
      case '3':
        while(true){
          puts("");
          spacer;
          do{
            printf("Which point do you want to move to? (1-%i)\n", points.size()); print_points(points);scanf("%i", &entry);}while(entry < 0 || entry > points.size());
            


          char add;
          do{
            puts("Do you want to move to other point?  Y / N");
            add = getch();
            add = tolower(add);
          }while(add != 'y' && add != 'n');
          if(add == 'n'){
            break;
          }
        }
        break;
      /*
      case 'P':
        puts("ok");
        break;*/
    }
  }while(mode2!='5');
}

void print_points(vector <class point> points){
  for(int i=0; i < points.size(); i++){
    spacer;
    printf("\nPoint %i:", i+1);
    printf("\n- ( %.2f , %.2f ) pos\n- ( %.2f , %.2f ) ang\n", points[i].x, points[i].y, points[i].ang1, points[i].ang2);
  }
}