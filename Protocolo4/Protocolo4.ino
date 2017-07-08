#include <Wire.h>

int id;
int id_siguiente;

int aleatorio;
#define ENVIAR_MENSAJE 0

int modulo_actual;
#define MODULO_INICIAL 20

int mensaje_recibido;
void receiveEvent(int HowMany){
  mensaje_recibido = Wire.read();
  Serial.println(mensaje_recibido);
}

void setup(){
  Serial.begin(9600);
  id = 2;
  id_siguiente = 3;

  randomSeed(analogRead(0));
  modulo_actual = MODULO_INICIAL;

  Wire.begin(id);
  Wire.onReceive(receiveEvent);
}

void loop(){
  Wire.beginTransmission(id_siguiente);
  Wire.write(0);
  
  aleatorio = random(modulo_actual);
  //Serial.println(aleatorio);

  if (aleatorio == ENVIAR_MENSAJE){
    modulo_actual = MODULO_INICIAL;
  }

  modulo_actual--;

  Wire.endTransmission();
}
