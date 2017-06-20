void setup() { 
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int sensorVal = digitalRead(7);
  
  Serial.println(sensorVal);
  
  if (sensorVal == HIGH) { 
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
