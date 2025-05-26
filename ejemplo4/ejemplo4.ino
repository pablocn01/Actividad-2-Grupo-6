const int dataPin = 12;   // DS
const int latchPin = 11; // ST_CP
const int clockPin = 9; // SH_CP
const int ldrPin = A0;

int ledLevel = 0;
const int maxLevel = 8;
const int minLevel = 0;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  ldrValue = 1023 - ldrValue;
  float lightPercent = map(ldrValue, 0, 1023, 0, 100);

  Serial.print("Luz: ");
  Serial.print(lightPercent);
  Serial.println("%");

  if (lightPercent < 80 && ledLevel < maxLevel) {
    ledLevel++;
    delay(100);
  } else if (lightPercent > 80 && ledLevel > minLevel) {
    ledLevel--;
    delay(100);
  }

  updateLEDs(ledLevel);
  delay(200);
}

void updateLEDs(int level) {
  byte ledValue = 0;
  for (int i = 0; i < level; i++) {
    ledValue |= (1 << i);
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledValue);
  digitalWrite(latchPin, HIGH);
}
