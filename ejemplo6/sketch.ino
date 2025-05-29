int valorLDR = 0;
byte i;

void setup() {
  // LEDs conectados a los pines 8, 9, 10, 11 y 12
  for (i = 8; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }

}

void loop() {
  valorLDR = analogRead(A0); // Leer el valor de la LDR en el pin A0

  // Apagar todos los LEDs al inicio
  for (i = 8; i <= 12; i++) {
    digitalWrite(i, LOW);
  }

// Apagamos todos los LEDs primero
  
if (valorLDR > 900) {  // Oscuridad alta, todos LEDs ON
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
}
else if (valorLDR > 700) {  // Oscuridad media-alta, 4 LEDs
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
}
else if (valorLDR > 400) {  // Oscuridad media, 3 LEDs
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
}
else if (valorLDR > 250) {  // Oscuridad baja, 2 LEDs
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
}
else if (valorLDR > 100) {  // Luz, solo 1 LED
  digitalWrite(12, HIGH);
}
else {
  digitalWrite(12, HIGH);
  
}

  
  
  delay(100); // retardo para estabilidad del circuito
}

