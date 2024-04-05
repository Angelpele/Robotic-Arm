#include <Stepper.h>
 
// Esto es el número de pasos por revolución
#define STEPS 4096 
// Número de pasos que queremos que de
#define NUMSTEPS 100

int v = 0;
int p = 1;
 
// Constructor, pasamos STEPS y los pines donde tengamos conectado el motor
Stepper stepper(STEPS, 8, 9, 10, 11);
 
void setup() {
  // Asignamos la velocidad en RPM (Revoluciones por Minuto)
  stepper.setSpeed(5);
  Serial.begin(115200);
  
}
 
void loop() {
  // Movemos el motor un número determinado de pasos
  for (int v = 0; v <= 200; v++){
  
   stepper.step(NUMSTEPS);
   int NUMBERSTEPS = NUMBERSTEPS + 10;
  Serial.println(NUMBERSTEPS);
  delay(10);  
 }

} 
