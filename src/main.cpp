/*
 * Auteurs: Jean-Samuel Lauzon
 * Date: Fevrier 2022
*/

/*------------------------------ Librairies ---------------------------------*/
#include <Arduino.h>
#include <ArduinoJson.h>
#include <math.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 9600        // Frequence de transmission serielle
#define TRIG_LEFT = 39
#define TRIG_RIGHT = 41
#define BUTTON_JSTICK = 43
#define DPAD_UP = 45
#define DPAD_LEFT = 47
#define DPAD_DOWN = 49
#define DPAD_RIGHT = 51

#define ACC_ST = 30
#define BAR_10 = 32
#define BAR_9 = 34
#define BAR_8 = 36
#define BAR_7 = 38
#define BAR_6 = 40
#define BAR_5 = 42
#define BAR_4 = 44
#define BAR_3 = 46
#define BAR_2 = 48
#define BAR_1 = 50

#define VERT_JSTICK = A0
#define HORI_JSTICK = A1
#define ACC_X = A2
#define ACC_Y = A3
#define ACC_Z = A4

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


/*------------------------- Prototypes de fonctions -------------------------*/
void sendMsg();
bool readMsg();
void serialEvent();
void bargraph(int nbBar);
void j_stick();
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
  if(readMsg){
    
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
  doc["vert_jstick"] = vert_jstick;
  doc["hori_jstick"] = hori_jstick;

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
    digitalWrite(pinLED,doc["led"].as<bool>());
    digitalWrite(BAR_10, );
    digitalWrite(BAR_9, );
    digitalWrite(BAR_8, );
    digitalWrite(BAR_7, );
    digitalWrite(BAR_6, );
    digitalWrite(BAR_5, );
    digitalWrite(BAR_4, );
    digitalWrite(BAR_3, );
    digitalWrite(BAR_2, );
    digitalWrite(BAR_1, );
  }
  return true;
}

void j_stick(){
  int vert_jstick = analogRead(VERT_JSTICK);
  bool vert_sign = true;
  int hori_jstick = analogRead(HORI_JSTICK);
  bool hori_sign = true;
  double angle = 0;
  
  if(vert_jstick > 400 && vert_jstick < 624)
    vert_jstick = 0;
  else if(vert_jstick < 400){
    vert_jstick = vert_jstick - 401;
    vert_sign = false;
  }
  else()
    vert_jstick = vert_jstick - 623;

  if(hori_jstick > 400 && hori_jstick < 624)
    hori_jstick = 0;
  else if(hori_jstick < 400){
    hori_jstick = hori_jstick - 401;
    hori_sign = false;
  }
  else()
    hori_jstick = hori_jstick - 623;
  
  if(vert_jstick == 0 && hori_jstick == 0)
    return angle_jstick;
  else(){
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