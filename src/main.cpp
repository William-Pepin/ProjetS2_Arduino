/*
 * Auteurs: Jean-Samuel Lauzon
 * Date: Fevrier 2022
*/

/*------------------------------ Librairies ---------------------------------*/
#include <Arduino.h>
#include "ArduinoJson.h"
#include <math.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 9600        // Frequence de transmission serielle
#define TRIG_LEFT 39
#define TRIG_RIGHT 41
#define BUTTON_JSTICK 43
#define DPAD_UP 45
#define DPAD_LEFT 47
#define DPAD_DOWN 49
#define DPAD_RIGHT 51

#define ACC_ST 30
#define BAR_10 32
#define BAR_9 34
#define BAR_8 36
#define BAR_7 38
#define BAR_6 40
#define BAR_5 42
#define BAR_4 44
#define BAR_3 46
#define BAR_2 48
#define BAR_1 50

#define VERT_JSTICK A0
#define HORI_JSTICK A1
#define ACC_X A2
#define ACC_Y A3
#define ACC_Z A4

#define JOYDRIFT 25
/*---------------------------- Variables globales ---------------------------*/

int bargraph = 0;


bool dpad_up = false;
bool dpad_down = false;
bool dpad_left = false;
bool dpad_right = false;

bool trig_left = false;
bool trig_right = false;

bool button_jstick = false;
double angle_jstick = 0;

bool acc_ST = false;
int acc_x = 0; //La valeur max ne sera pas 1024 étant donné que l'accéléromètre est alimenté par du 3.3V
int acc_y = 0;
int acc_z = 0;

double angle = 0;


/*------------------------- Prototypes de fonctions -------------------------*/
void sendMsg();
bool readMsg();
double j_stick();
double j_stick_MAX();
void bargraphPinSetup(int nbBar);
void buttons();
/*---------------------------- Fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // Initialisation de la communication serielle
  pinMode(BAR_1, OUTPUT);
  pinMode(BAR_2, OUTPUT);
  pinMode(BAR_3, OUTPUT);
  pinMode(BAR_4, OUTPUT);
  pinMode(BAR_5, OUTPUT);
  pinMode(BAR_6, OUTPUT);
  pinMode(BAR_7, OUTPUT);
  pinMode(BAR_8, OUTPUT);
  pinMode(BAR_9, OUTPUT);
  pinMode(BAR_10, OUTPUT);
  pinMode(ACC_ST, OUTPUT);

  pinMode(TRIG_LEFT, INPUT_PULLUP);
  pinMode(TRIG_RIGHT, INPUT_PULLUP);

  pinMode(DPAD_UP, INPUT_PULLUP);
  pinMode(DPAD_DOWN, INPUT_PULLUP);
  pinMode(DPAD_LEFT, INPUT_PULLUP);
  pinMode(DPAD_RIGHT, INPUT_PULLUP);

  pinMode(BUTTON_JSTICK, INPUT_PULLUP);

  digitalWrite(BAR_1, LOW);
  digitalWrite(BAR_2, LOW);
  digitalWrite(BAR_3, LOW);
  digitalWrite(BAR_4, LOW);
  digitalWrite(BAR_5, LOW);
  digitalWrite(BAR_6, LOW);
  digitalWrite(BAR_7, LOW);
  digitalWrite(BAR_8, LOW);
  digitalWrite(BAR_9, LOW);
  digitalWrite(BAR_10, LOW);
  digitalWrite(ACC_ST, LOW);
}

/* Boucle principale (infinie) */
void loop() {
  //Serial.println(digitalRead(DPAD_RIGHT));

  /* if (digitalRead(DPAD_RIGHT) == LOW)
  {
    Serial.println("LOW");
  }
  
  if (digitalRead(DPAD_RIGHT) == HIGH)
  {
    Serial.println("HIGH");
  } */


  //if (digitalRead(TRIG_RIGHT) == LOW)
  //{
  //  Serial.println("Trig Right pressed");
  //}
//
  //if (digitalRead(DPAD_UP) == LOW)
  //{
  //  Serial.println("DPAD_UP pressed");
  //}
//
  //if (digitalRead(DPAD_DOWN) == LOW)
  //{
  //  Serial.println("DPAD_DOWN pressed");
  //}
//
  //if (digitalRead(DPAD_LEFT) == LOW)
  //{
  //  Serial.println("DPAD_LEFT pressed");
  //}
//
  //if (digitalRead(DPAD_RIGHT) == LOW)
  //{
  //  Serial.println("DPAD_RIGHT pressed");
  //}
//
  //if (digitalRead(BUTTON_JSTICK) == LOW)
  //{
  //  Serial.println("Trig Left pressed");
  //}

  Serial.print("angle JSTICK ");
  Serial.println(j_stick_MAX());
  
  bargraphPinSetup(10);
  
  delay(500);
  //Serial.println("ACC X = " + analogRead(ACC_X));
  //Serial.println("ACC Y = " + analogRead(ACC_Y));
  //Serial.println("ACC Z = " + analogRead(ACC_Z));
  
  /*if(readMsg){
    angle_jstick = j_stick();
    buttons();
    sendMsg();
  }*/
  delay(10);
}

/*---------------------------Definition de fonctions ------------------------*/

/*---------------------------Definition de fonctions ------------------------
Fonction d'envoi
Entrée : Aucun
Sortie : Aucun
Traitement : Envoi du message
-----------------------------------------------------------------------------*/
void sendMsg() {
  StaticJsonDocument<500> doc;
  // Elements du message
  doc["dpad_up"] = dpad_up;
  doc["dpad_down"] = dpad_up;
  doc["dpad_left"] = dpad_left;
  doc["dpad_right"] = dpad_right;

  doc["trig_left"] = trig_left;
  doc["trig_right"] = trig_right;

  doc["button_jstick"] = button_jstick;
  doc["angle_jstick"] = angle_jstick;

  doc["acc_ST"] = acc_ST;
  doc["acc_x"] = acc_x;
  doc["acc_y"] = acc_y;
  doc["acc_z"] = acc_z;

  // Serialisation
  serializeJson(doc, Serial);

  // Envoie
  Serial.println();
}

/*---------------------------Definition de fonctions ------------------------
Fonction de reception
Entrée : Aucun
Sortie : Aucun
Traitement : Réception du message
-----------------------------------------------------------------------------*/
bool readMsg(){
  // Lecture du message Json
  StaticJsonDocument<500> doc;
  JsonVariant parse_msg;

  // Lecture sur le port Seriel
  DeserializationError error = deserializeJson(doc, Serial);

  // Si erreur dans le message
  if (error) {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return false;
  }

  // Analyse des éléments du message message
  parse_msg = doc["bargraph"];
  if (!parse_msg.isNull()) {
    // mettre la led a la valeur doc["led"]
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, LOW);
    digitalWrite(BAR_4, LOW);
    digitalWrite(BAR_3, LOW);
    digitalWrite(BAR_2, LOW);
    digitalWrite(BAR_1, LOW);
    return false;
  }
  bargraphPinSetup(parse_msg);
  return true;
}

double j_stick_MAX(){
  int vert_jstick = analogRead(VERT_JSTICK);
  bool vert_sign = true;
  int hori_jstick = analogRead(HORI_JSTICK);
  bool hori_sign = true;
  
  
  Serial.print("VER = ");
  Serial.println(analogRead(VERT_JSTICK));
  Serial.print("HOR = ");
  Serial.println(analogRead(HORI_JSTICK)); 

  if ((vert_jstick >= (496-JOYDRIFT) && hori_jstick >= (509-JOYDRIFT)) && ((vert_jstick <= (496+JOYDRIFT) && hori_jstick <= (509+JOYDRIFT))))
  {
    angle = 420.69;
  }else
  {
    vert_jstick -= 512;
    hori_jstick -= 512;
    angle = atan2(vert_jstick, hori_jstick);
  }
  
  Serial.print("Angle = ");
  Serial.print(angle);
}


double j_stick(){
  int vert_jstick = analogRead(VERT_JSTICK);
  bool vert_sign = true;
  int hori_jstick = analogRead(HORI_JSTICK);
  bool hori_sign = true;
  double angle = 0;
  
  Serial.print("VER = ");
  Serial.println(analogRead(VERT_JSTICK));
  Serial.print("HOR = ");
  Serial.println(analogRead(HORI_JSTICK)); 
  
  if(vert_jstick > 400 && vert_jstick < 624)
    vert_jstick = 0;
  else if(vert_jstick < 400){
    vert_jstick = vert_jstick - 401;
    vert_sign = false;
  }
  else
    vert_jstick = vert_jstick - 623;

  if(hori_jstick > 400 && hori_jstick < 624)
    hori_jstick = 0;
  else if(hori_jstick < 400){
    hori_jstick = hori_jstick - 401;
    hori_sign = false;
  }
  else
    hori_jstick = hori_jstick - 623;
  
  if(vert_jstick == 0 && hori_jstick == 0)
    return angle_jstick;
  else{
    angle = abs(vert_jstick) / abs(hori_jstick);
    angle = atan(angle);
    if(vert_sign && !hori_sign)
      angle = PI - angle;
    else if(!vert_jstick && !hori_jstick)
      angle = PI + angle;
    else if(!vert_jstick && hori_jstick)
      angle = 2 * PI - angle;
    return angle;
  }
  
}

void bargraphPinSetup(int nbBar){
  switch (nbBar)
  {
  case 0 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, LOW);
    digitalWrite(BAR_4, LOW);
    digitalWrite(BAR_3, LOW);
    digitalWrite(BAR_2, LOW);
    digitalWrite(BAR_1, LOW);
    break;

  case 1 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, LOW);
    digitalWrite(BAR_4, LOW);
    digitalWrite(BAR_3, LOW);
    digitalWrite(BAR_2, LOW);
    digitalWrite(BAR_1, HIGH);
    break;
  
  case 2 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, LOW);
    digitalWrite(BAR_4, LOW);
    digitalWrite(BAR_3, LOW);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);
    break;
  
  case 3 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, LOW);
    digitalWrite(BAR_4, LOW);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 4 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, LOW);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 5 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, LOW);
    digitalWrite(BAR_5, HIGH);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 6 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, LOW);
    digitalWrite(BAR_6, HIGH);
    digitalWrite(BAR_5, HIGH);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 7 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, LOW);
    digitalWrite(BAR_7, HIGH);
    digitalWrite(BAR_6, HIGH);
    digitalWrite(BAR_5, HIGH);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 8 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, LOW);
    digitalWrite(BAR_8, HIGH);
    digitalWrite(BAR_7, HIGH);
    digitalWrite(BAR_6, HIGH);
    digitalWrite(BAR_5, HIGH);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 9 :
    digitalWrite(BAR_10, LOW);
    digitalWrite(BAR_9, HIGH);
    digitalWrite(BAR_8, HIGH);
    digitalWrite(BAR_7, HIGH);
    digitalWrite(BAR_6, HIGH);
    digitalWrite(BAR_5, HIGH);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  case 10 :
    digitalWrite(BAR_10, HIGH);
    digitalWrite(BAR_9, HIGH);
    digitalWrite(BAR_8, HIGH);
    digitalWrite(BAR_7, HIGH);
    digitalWrite(BAR_6, HIGH);
    digitalWrite(BAR_5, HIGH);
    digitalWrite(BAR_4, HIGH);
    digitalWrite(BAR_3, HIGH);
    digitalWrite(BAR_2, HIGH);
    digitalWrite(BAR_1, HIGH);  
    break;
  
  default:
    break;
  }
}

void buttons(){
  dpad_up = digitalRead(DPAD_UP);
  dpad_down = digitalRead(DPAD_DOWN);
  dpad_left = digitalRead(DPAD_LEFT);
  dpad_right = digitalRead(DPAD_RIGHT);

  trig_left = digitalRead(TRIG_LEFT);
  trig_right = digitalRead(TRIG_RIGHT);

  button_jstick = digitalRead(BUTTON_JSTICK);

}
