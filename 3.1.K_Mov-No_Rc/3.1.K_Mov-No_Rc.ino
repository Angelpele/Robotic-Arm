#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include "printf.h"
#include "math.h"

#define arm1 = 120
#define arm2 = 135
#define DATARATE RF24_2MBPS
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

RF24 radio(2, 3);
int ang1, ang2, ang3, ang4, anginf;
/*
int girogiractual = 120;
int giroinfactual = 120;
int girosupactual = 140;
int girosupextactual = 120;
int girogirbaseactual;
*/

Servo servoinf, servosup, servosupext, servogir, servogirbase;
void posIV(int* ang1, int* ang2);

void setup()
{
  Serial.begin(115200);
  printf_begin();
  servogir.attach(22);
  servosup.attach(25);
  servoinf.attach(23);
  servosupext.attach(26);
  servogirbase.attach(30);
}

void loop()
{

  /*
   girogir = map(received_data.y_ang, 0, 255, 0, 180);
   girosupext = map(received_data.x_ang, 0, 255, 0, 180);
   girosup = map(received_data.y_ang2, 0, 255, 0, 180);
   giroinf = map(received_data.x_ang2, 0, 255, 0, 180);
   
  */

 Serial.print("ang1: %iº --- ang2: %iº --- ang3: %iº --- ang4: %iº --- anging: %iº", ang1,ang2,ang3,ang4,anginf);

  
}



