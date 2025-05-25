#include <Servo.h>

Servo myservo;  // crear objeto servo para controlar un servo
// Se pueden crear hasta doce objetos servo en la mayoría de placas

int pos = 0;    // variable para almacenar la posición del servo

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // adjuntar el servo al pin 9
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // va de 0 a 180 grados
    myservo.write(pos);  // indica al servo que vaya a la posición 'pos'
    delay(15);           // espera 15 ms para que el servo alcance la posición
  }
  
  for (pos = 180; pos >= 0; pos -= 1) { // va de 180 a 0 grados
    myservo.write(pos);
    delay(15);
  }
}
