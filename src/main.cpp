/*
 * Auteurs: Jean-Samuel Lauzon
 * Date: Fevrier 2022
 */

/*------------------------------ Librairies ---------------------------------*/
#include <Arduino.h>
#include "ArduinoJson.h"
#include <math.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 19200       // Frequence de transmission serielle
#define TRIG_LEFT 41
#define TRIG_RIGHT 39
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
#define SHAKE_TRIG 20
/*---------------------------- Variables globales ---------------------------*/

int bargraph = 0;

bool dpad_up = false;
bool dpad_down = false;
bool dpad_left = false;
bool dpad_right = false;

bool trig_left = false;
bool trig_right = false;

bool button_jstick = false;

int d_u,d_d,d_l,d_r,t_l,t_r,b_j;

int angle_jstick = 0;

bool acc_ST = false;
int acc_x1 = 0; // La valeur max ne sera pas 1024 étant donné que l'accéléromètre est alimenté par du 3.3V
int acc_y1 = 0;
int acc_z1 = 0;
int acc_x2 = 0; // La valeur max ne sera pas 1024 étant donné que l'accéléromètre est alimenté par du 3.3V
int acc_y2 = 0;
int acc_z2 = 0;
int acc_x3 = 0; // La valeur max ne sera pas 1024 étant donné que l'accéléromètre est alimenté par du 3.3V
int acc_y3 = 0;
int acc_z3 = 0;
int acc = 0;

double angle = 0;

volatile bool shouldRead_ = false; // Drapeau prêt à lire un message

/*------------------------- Prototypes de fonctions -------------------------*/
void sendMsg();
bool readMsg();
double j_stick();
double j_stick_MAX();
void bargraphPinSetup(int nbBar);
void buttons();
void showButtonpressed();
void serialEvent();
int acc_shake();
/*---------------------------- Fonctions "Main" -----------------------------*/

void setup()
{
  Serial.begin(BAUD); // Initialisation de la communication serielle
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
  
  if(shouldRead_){
    readMsg();
    angle_jstick = j_stick();
    acc = acc_shake();
    buttons();
    sendMsg();
  }
}

/*---------------------------Definition de fonctions ------------------------*/
void serialEvent() { shouldRead_ = true; }
/*---------------------------Definition de fonctions ------------------------
Fonction d'envoi
Entrée : Aucun
Sortie : Aucun
Traitement : Envoi du message
-----------------------------------------------------------------------------*/
void sendMsg()
{
  StaticJsonDocument<500> doc;
  // Elements du message
  d_u = !dpad_up;
  d_d = !dpad_down;
  d_l = !dpad_left;
  d_r = !dpad_right;
  t_l = !trig_left;
  t_r = !trig_right;
  b_j = !button_jstick;
  
  doc["d_u"] = d_u;
  doc["d_d"] = d_d;
  doc["d_l"] = d_l;
  doc["d_r"] = d_r;

  doc["t_l"] = t_l;
  doc["t_r"] = t_r;

  doc["b_j"] = b_j;
  doc["a_j"] = angle_jstick;

  doc["acc"] = acc;

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
bool readMsg()
{
  // Lecture du message Json
  StaticJsonDocument<500> doc;
  JsonVariant parse_msg;

  // Lecture sur le port Seriel
  DeserializationError error = deserializeJson(doc, Serial);
  shouldRead_ = false;
  // Si erreur dans le message
  if (error)
  {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Analyse des éléments du message message
  
  parse_msg = doc["bg"];
  //acc_ST = doc["a_S"];
  if (!parse_msg.isNull()) {
    return false;
  }
}

double j_stick()
{
  int vert_jstick = analogRead(VERT_JSTICK);
  bool vert_sign = true;
  int hori_jstick = analogRead(HORI_JSTICK);
  bool hori_sign = true;

  if ((vert_jstick >= (496 - JOYDRIFT) && hori_jstick >= (509 - JOYDRIFT)) && ((vert_jstick <= (496 + JOYDRIFT) && hori_jstick <= (509 + JOYDRIFT))))
  {
    return angle;
  }
  else
  {
    vert_jstick -= 512;
    hori_jstick -= 512;
    angle = atan2(vert_jstick, hori_jstick);
  }

  if(angle < 0)
    angle = ((PI + angle)/(2*PI)) * 360;
  else
    angle = ((abs(angle)/(2*PI)) * 360) + 180;

  return angle;
}

void bargraphPinSetup(int nbBar)
{
  switch (nbBar)
  {
  case 0:
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

  case 1:
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

  case 2:
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

  case 3:
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

  case 4:
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

  case 5:
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

  case 6:
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

  case 7:
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

  case 8:
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

  case 9:
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

  case 10:
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

void buttons()
{
  dpad_up = digitalRead(DPAD_UP);
  dpad_down = digitalRead(DPAD_DOWN);
  dpad_left = digitalRead(DPAD_LEFT);
  dpad_right = digitalRead(DPAD_RIGHT);

  trig_left = digitalRead(TRIG_LEFT);
  trig_right = digitalRead(TRIG_RIGHT);

  button_jstick = digitalRead(BUTTON_JSTICK);
}

int acc_shake()
{
  bool shake_x = false;
  bool shake_y = false;
  bool shake_z = false;

  acc_x3 = acc_x2;
  acc_y3 = acc_y2;
  acc_z3 = acc_z2;

  acc_x2 = acc_x1;
  acc_y2 = acc_y1;
  acc_z2 = acc_z1;

  acc_x1 = analogRead(ACC_X);
  acc_y1 = analogRead(ACC_Y);
  acc_z1 = analogRead(ACC_Z);

  int calc_x = (abs(acc_x1-acc_x2)+abs(acc_x2-acc_x3))/2;
  int calc_y = (abs(acc_y1-acc_y2)+abs(acc_y2-acc_y3))/2;
  int calc_z = (abs(acc_z1-acc_z2)+abs(acc_z2-acc_z3))/2;

  if(calc_x >= SHAKE_TRIG || calc_y >= SHAKE_TRIG || calc_z >= SHAKE_TRIG)
    return 1;
  else
    return 0;
};