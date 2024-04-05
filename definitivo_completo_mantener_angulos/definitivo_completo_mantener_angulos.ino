#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define DATARATE RF24_2MBPS
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

#include "printf.h"

RF24 radio(2, 3);
int x;
int girogir;
int giroinf;
int girosup;
int girosupext;
int girogirbase;

int girogiractual = 120;
int giroinfactual = 120;
int girosupactual = 140;
int girosupextactual = 120;
int girogirbaseactual;
Servo servoinf;
Servo servosup;
Servo servosupext;
Servo servogir;
Servo servogirbase;

struct Received_data {
   byte ESTADObut;
   byte PIN_SW;  // botón joy 1
   byte PIN_SW2;  // botón joy 2
   byte x_ang;   //ang joy x 1
   byte y_ang;  //ang joy y 1
   byte x_ang2;  //ang joy x 2
   byte y_ang2;  //ang joy y 2
   byte ang_x; //mpu
   byte ang_y;  //mpu
};

Received_data received_data;

void reset_the_Data() 
{
  // SEGURIDAD cuando no recibe datos del TX
  received_data.x_ang = 124;   
  received_data.y_ang = 124;
  received_data.x_ang2 = 50;
  received_data.y_ang2 = 124;
  received_data.PIN_SW = 0;
  received_data.PIN_SW2 = 0;
  received_data.ang_x = 125;
  received_data.ang_y = 125;
 
  received_data.ESTADObut = 0;
  
}

void setup()
{
  Serial.begin(115200);
  printf_begin();
  servogir.attach(22);
  servosup.attach(25);
  servoinf.attach(23);
  servosupext.attach(26);
  servogirbase.attach(30);
  
  
  //reset_the_Data();
 
  radio.begin();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.setDataRate( DATARATE ) ;
  radio.setPALevel( RF24_PA_MAX ) ;
  radio.setChannel(0x34);
  radio.enableDynamicPayloads() ;
  radio.enableAckPayload();               // not used here
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setAutoAck( true ) ;
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  radio.powerUp();
  radio.startListening();
}


unsigned long lastRecvTime = 0;

void receive_the_data()
{
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  lastRecvTime = millis(); 
  }
}

void loop()
{
   Serial.print(F("Rotacion en X:  "));
   Serial.print(received_data.ang_x);
   Serial.print(F("\t Rotacion en Y: "));
   Serial.print(received_data.ang_y);
   //Serial.print(F("\t Rotacion en Z: "));
   //Serial.println(ang_z);
   
   Serial.print("\t X_ang:" );
   Serial.print(received_data.x_ang);
   Serial.print("  ");
   Serial.print( "Y_ang:" );
   Serial.print(received_data.y_ang);
   Serial.print( " Botjoy:");
   Serial.print(received_data.PIN_SW);
   Serial.print( "   X_ang2:" );
   Serial.print(received_data.x_ang2);
   Serial.print("  ");
   Serial.print( "Y_ang2:" );
   Serial.print(received_data.y_ang2);
   Serial.print( " Botjoy2:");
   Serial.print(received_data.PIN_SW2);
 
   
   Serial.print(F("  Botón switch: "));
   Serial.println(received_data.ESTADObut);

   girogir = map(received_data.y_ang, 0, 255, 0, 180);
   girosupext = map(received_data.x_ang, 0, 255, 0, 180);
   girosup = map(received_data.y_ang2, 0, 255, 0, 180);
   giroinf = map(received_data.x_ang2, 0, 255, 0, 180);
   
   
   if (girogir >= 120){
    girogiractual ++;
    servogir.write(girogiractual);
    delay(10);
   }
   if (girogir <= 70){
    girogiractual --;
    servogir.write(girogiractual);
    delay(10);
   }

 //-------------------------------------------------------//  
   if (girosup >= 120){
    girosupactual ++;
    servosup.write(girosupactual);
    delay(10);
   }
   if (girosup <= 70){
    girosupactual --;
    servosup.write(girosupactual);
    delay(10);
   }
 
 //-------------------------------------------------------//    
   if (girosupext >= 120){
    girosupextactual ++;
    servosupext.write(girosupextactual);
    delay(10);
   }
   if (girosupext <= 70){
    girosupextactual --;
    servosupext.write(girosupextactual);
    delay(10);
   }

 //-------------------------------------------------------//  

   if (giroinf >= 120){
    giroinfactual ++;
    servoinf.write(giroinfactual);
    delay(10);
   }
   if (giroinf <= 70){
    giroinfactual --;
    servoinf.write(giroinfactual);
    delay(10);
   }

 //-------------------------------------------------------//  
    // girogir base //

   if (received_data.PIN_SW == 1){
    girogirbase ++;
    servogirbase.write(girogirbase);

   }
   if (received_data.PIN_SW2 == 1){
    girogirbase --;
    servogirbase.write(girogirbase);

   }


    
   receive_the_data();

// si se pierde la señal por 1 segundo
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    reset_the_Data();
    Serial.println("Fail...............................................");
    
  } 
}
