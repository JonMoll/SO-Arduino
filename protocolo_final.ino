#include <QueueArray.h>
#include <Wire.h>

int id;
int id_siguiente;

// EL ARDUINO SOLO VA A PODER HACER UNA COSA A LA VEZ
// O ESTA LEYENDO O ESTA ESCRIBIENDO
// PARA SABER QUE ESTA HACIENDO VAMOS A USAR LA VARIABLE estado
int estado;
#define LIBRE 0
#define LEYENDO 1
#define ESCRIBIENDO 2
#define LEYENDO_GUARDADO 3
#define PASANDO 4

// VAMOS A DORMIR LAS TAREAS DURANTE UN PERIODO DE TIEMPO
// PARA QUE LOS println SEAN VISIBLES
#define DORMIR 1500

// VAMOS A ENVIAR UN MENSAJE CADA VES QUE LA VARIABLE aleatorio SEA CERO
int aleatorio;
#define ENVIAR_MENSAJE 0

// PARA ASEGURARNOS QUE SIEMPRE SE ENVIE UN MENSAJE EN ALGUN TIEMPO ALEATORIO
// VAMOS A USAR LA VARIABLE modulo_actual QUE VA A IR DECRECIENDO
// ASI TENDREMOS LA SEGURIDAD DE QUE EN ALGUN MOMENTO LA VARIABLE aleatorio SEA CERO
int modulo_actual;
#define MODULO_INICIAL 20

// EN ESTA VARIABLE VAMOS A GUARDAR EL MENSAJE QUE RECIBAMOS
int mensaje_recibido;
#define NINGUNO 0

// VAMOS A TENER UNA COLA QUE ALMACENE CUANTOS MENSAJES PENTIENTES TENEMOS POR LEER
QueueArray<int> pendiente_leer;

// VAMOS A TENER UNA VARIABLE QUE ALMACENE CUANTOS MENSAJES PENTIENTES TENEMOS POR ENVIAR
int pendiente_enviar;

// EN ESTA VARIABLE SE VA A GUARDAR QUE MENSAJE SE VA A PASAR
int pasar_mensaje;

int mensaje_anterior = NINGUNO;
void receiveEvent(int HowMany){
  /*if (Wire.read() != mensaje_anterior){
    mensaje_recibido = Wire.read();
    mensaje_anterior = mensaje_recibido;
  }*/

  Serial.println(Wire.read());
  
  if (Wire.read() == id){
    Serial.println("<< Recorrido completado !!!!!!!!!!");
  }
  
  mensaje_recibido = Wire.read();
  //Serial.println(mensaje_recibido);
}

void setup() {
  Serial.begin(9600);
  id = 2;
  id_siguiente = 1;
  estado = LIBRE;
  mensaje_recibido = NINGUNO;
  pasar_mensaje = NINGUNO;
  pendiente_enviar = 0;

  randomSeed(analogRead(0));
  modulo_actual = MODULO_INICIAL;

  Wire.begin(id);
  Wire.onReceive(receiveEvent);
}

void loop() {
  Wire.beginTransmission(id_siguiente);
  Wire.write(NINGUNO);
  
  Serial.println("...");

  if (estado == PASANDO){
    Wire.write(pasar_mensaje);
    Wire.write(pasar_mensaje);
    Wire.write(pasar_mensaje);
    Wire.write(pasar_mensaje);
    Wire.write(pasar_mensaje);
    Serial.println(">> Se ha PASADO:");
    Serial.println(pasar_mensaje);
    estado = LIBRE;
  }

  if (estado == ESCRIBIENDO){
    Wire.write(id);
    Wire.write(id);
    Wire.write(id);
    Wire.write(id);
    Wire.write(id);
    Serial.println(">> Se ha ENVIADO");
    estado = LIBRE;
  }

  // LEYENDO MENSAJE GUARDADOS --------------------------------------------------
  if (!pendiente_leer.isEmpty()){
    Serial.println("<< Leyendo mensaje guardado");

    if (estado == LIBRE){
      Serial.println("<< Mensaje guardado:");
      int mensaje_cola = pendiente_leer.dequeue();
      Serial.println(mensaje_cola);

      if (mensaje_cola == id){
        Serial.println("<< Recorrido completado !!!!!!!!!!");
      }
      else{
        if (mensaje_cola == 1 or mensaje_cola == 2){
          Serial.println(">> Pasando mensaje");
          pasar_mensaje = mensaje_cola;
          estado = PASANDO;
        }
      }
    }
    else{
      Serial.println("<< Ocupado, se leera luego");
    }
  }

  // LEYENDO MENSAJES --------------------------------------------------
  if (mensaje_recibido != NINGUNO){
    Serial.println("    << Leyendo mensaje");

    if (estado == LIBRE){
      Serial.println("    << Mensaje:");
      Serial.println(mensaje_recibido);

      if (mensaje_recibido == id){
        Serial.println("    << Recorrido completado !!!!!!!!!!");
      }
      else{
        if (mensaje_recibido == 1 or mensaje_recibido == 2){
          Serial.println("    >> Pasando mensaje");
          pasar_mensaje = mensaje_recibido;
          estado = PASANDO;
        }
      }
    }
    else{
      Serial.println("    << Ocupado, se leera luego");
      pendiente_leer.enqueue(mensaje_recibido);
    }

    mensaje_recibido = NINGUNO;
  }

  // ENVIANDO MENSAJES PENDIENTES --------------------------------------------------
  if (pendiente_enviar > 0){
    Serial.println("        >> Enviando mensaje pendiente");

    if (estado == LIBRE){
      estado = ESCRIBIENDO;
      pendiente_enviar--;
    }
    else{
      Serial.println("        >> Ocupado, se enviara luego");
    }
  }

  // ENVIANDO MENSAJES --------------------------------------------------
  aleatorio = random(modulo_actual);
  //Serial.println(aleatorio);

  if (aleatorio == ENVIAR_MENSAJE)
  {
    Serial.println("            >> Enviando mensaje");

    if (estado == LIBRE){
      estado = ESCRIBIENDO;
    }
    else{
      Serial.println("            >> Ocupado, se enviara luego");
      pendiente_enviar++;
    }

    modulo_actual = MODULO_INICIAL; // REINICIAMOS EL MODULO
  }

  modulo_actual--;

  Wire.endTransmission();
  delay(DORMIR);
}
