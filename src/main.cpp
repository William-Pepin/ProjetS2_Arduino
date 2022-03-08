#include <Arduino.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 9600        // Frequence de transmission serielle

int incomingByte = 0; // for incoming serial data
char msg[1000];
int i = 0;

/*---------------------------- Fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // Initialisation de la communication serielle
}

/* Boucle principale (infinie) */
void loop() {
    // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    if(incomingByte != 0){
        msg[i] = incomingByte;
        i++;
    }
    if(incomingByte == 10){
        delay(10);
        Serial.println(msg);
    }
  }
}
