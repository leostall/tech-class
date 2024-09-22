#include <Stepper.h> 

const int passos = 2048; 
char sentido;

Stepper myStepper(passos, 8, 10, 9, 11);

void setup() {
 Serial.begin(9600);
}

void loop() {
 if (Serial.available()) { 
   sentido = Serial.read();
   Serial.println(sentido);
 }
 switch (sentido) {
  case 'a':
  abrir();
  break;
  case 'b':
  fechar();
  break;
 }
}

void abrir() {
 Serial.println("RECOLHENDO TELA");
 myStepper.setSpeed(13);
 myStepper.step(passos);
}

void fechar() {
 Serial.println("ABRINDO TELA");
 myStepper.setSpeed(15);
 myStepper.step(-passos);
}

