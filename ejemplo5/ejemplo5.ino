int valorrcds;
int brilloLED;
const int ledPin = 5;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  valorrcds = analogRead(0);
  Serial.println(valorrcds);

  valorrcds = 1023 - valorrcds;

  int brilloLED = map(valorrcds, 0, 1023, 0, 255);
  analogWrite(ledPin, brilloLED);
  delay(100);

}
