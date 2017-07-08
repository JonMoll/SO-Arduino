#include <Wire.h>

int id;
int id_siguiente;

int aleatorio;
#define ENVIAR_MENSAJE 0

int modulo_actual;
#define MODULO_INICIAL 20

int mensaje_recibido;
void receiveEvent(int HowMany){
  Serial.println(Wire.read());
  //mensaje_recibido = Wire.read();
  //Serial.println(mensaje_recibido);
}

void setup(){
  Serial.begin(9600);
  id = 1;
  id_siguiente = 2;

  randomSeed(analogRead(5));
  modulo_actual = MODULO_INICIAL;

  Wire.begin(id);
  Wire.onReceive(receiveEvent);
  Wire.beginTransmission(id_siguiente);
}

void loop(){
  Wire.write(1);
  
  aleatorio = random(modulo_actual);
  //Serial.println(aleatorio);

  if (aleatorio == ENVIAR_MENSAJE){
    modulo_actual = MODULO_INICIAL;
  }

  modulo_actual--;

  //Wire.endTransmission();
}
