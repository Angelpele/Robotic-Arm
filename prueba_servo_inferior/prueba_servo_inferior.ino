#include <Servo.h>
Servo motor1;
int angservo;
void setup() {
  // put your setup code here, to run once:
  motor1.attach(7);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  angservo = motor1.read();
  Serial.println(angservo);

     for (int i = 0; i <= 50; i++) {
    // Desplazamos al ángulo correspondiente
    motor1.write(i);
    // Hacemos una pausa de 25ms
    delay(100);
  }
  for (int i = 50; i > 0; i--){
    motor1.write(i);
    delay(60);
  }
}
