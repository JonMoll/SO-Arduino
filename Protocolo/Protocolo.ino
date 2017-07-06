int comunicacion_siguiente = 3;

void setup() {
  Serial.begin(9600);
  pinMode(comunicacion_siguiente, OUTPUT);
}

void loop() {
  Serial.println("[A1] Esperando...");
  analogWrite(comunicacion_siguiente, 3);
  
  delay(1000);
}
