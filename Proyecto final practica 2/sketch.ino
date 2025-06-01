#include <LiquidCrystal.h>
#include <DHT.h>
#include <Servo.h>
#include <MPU6050.h>

// Pines para la LCD
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DHT22
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LDR
const int ldrPin = A0;

// sensor de calidad de aire
#define AIR_QUALITY_PIN A2

// Relé
const int relayPin = 6;
const float temperatureThreshold = 15;

// Variables temperatura
// Calcular cada 15 segundos la temperatura y media cada 2 minutos
float dhtTemps[8];
int dhtIndex = 0;
int dhtCount = 0;  // lecturas validas
unsigned long lastDHTRead = 0;
const unsigned long DHT_INTERVAL = 15000; // 15 segundos

//servo
Servo myservo;  // crear objeto servo para controlar un servo
int pos = 0;    // variable para almacenar la posición del servo

// Gyroscope
MPU6050 mpu;  // Sensor MPU6050

// Cambio de visualizacion
int displayMode = 0; 

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Leyendo Sensores...");
  dht.begin();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  myservo.attach(3);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  mpu.initialize();

}



// funcion string sencilla para ver si la calidad es buena o mala
String calidadAireTexto(int valor) {
  if (valor > 700) return "Malo";
  if (valor > 500) return "Regular";
  return "Bueno";
}



boolean position = true;
void loop() {

  if (position == true){
    myservo.write(0);
    position = false;
  }

  // DHT22
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  // LDR
  int ldrValue = 1023 - analogRead(ldrPin);
  int lightLevel = map(ldrValue, 0, 1023, 0, 100); // Invierte el rango: más luz -> mayor valor


  // Lectura calidad del aire 
  int airQualityRaw = analogRead(AIR_QUALITY_PIN);


  // Giroscopio
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);



  // Control del relé basado en la temperatura del DHT22
  if (!isnan(temperatureC)) { // Verifica que lectura del DHT
    if (temperatureC < temperatureThreshold) {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relé ACTIVADO (Temperatura < 20ºC)");
    } else {
      digitalWrite(relayPin, LOW);  // Desactiva el relé
      Serial.println("Relé DESACTIVADO (Temperatura >= 20ºC)");
    }
  }

  if ( lightLevel < 25) {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  } else if ( lightLevel < 50) {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  } else {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }


  // Calcular promedio cada 4 lecturas
  static unsigned long lastAverageTime = 0;
  // en loop()
  if (millis() - lastDHTRead >= DHT_INTERVAL) {
    lastDHTRead = millis();

    float tempDHT = dht.readTemperature();

    if (!isnan(tempDHT)) {
      dhtTemps[dhtIndex] = tempDHT;
      dhtIndex = (dhtIndex + 1) % 8;

      if (dhtCount < 8) { //va hasta 8 porque si hay mediciones cada 15 segundos, hasta 2 minutos hay 8 mediciones
        dhtCount++;
      }

      // Calcular promedio cada 8 lecturas (2 minutos)
      if (dhtCount == 8) {
        float sum = 0;
        for (int i = 0; i < 8; i++) {
          sum += dhtTemps[i];
        }
        float promedio = sum / 8;

        // Mostrar promedio en el LCD durante 5 segundos
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Prom: ");
        lcd.print(promedio, 1);
        lcd.print(" C");

        lcd.setCursor(1, 1);
        lcd.print("Aire:");
        lcd.print(calidadAireTexto(airQualityRaw));

        delay(5000); // Mostrar durante 5 segundos

        dhtCount = 0; // Resetear el contador para próximas 8 lecturas
      }


      if (tempDHT <= 30 && pos == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp OK ");
        lcd.print(tempDHT, 1);
        lcd.print(" C");
        delay(1000);
        pos = 0;
        myservo.write(0);
      } else if(tempDHT <= 30 && pos != 0) {
        for (pos = 180; pos >= 0; pos -= 1) { // va de 180 a 0 grados
          myservo.write(pos);
          delay(15);
        }
        pos = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("cerrando ");
        lcd.setCursor(0, 1);
        lcd.print("compuertas ");
        delay(1000);
      } else if(tempDHT > 30 && pos == 0 ) {
        for (pos = 0; pos <= 180; pos += 1) { // va de 0 a 180 grados
          myservo.write(pos);  // indica al servo que vaya a la posición 'pos'
          delay(15);           // espera 15 ms para que el servo alcance la posición
        }
        pos = 180;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("abriendo ");
        lcd.setCursor(0, 1);
        lcd.print("compuertas ");
        delay(1000);
      } else if(tempDHT > 30 && pos != 0 ) {
      
        pos = 180;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("compuertas ya");
        lcd.setCursor(0, 1);
        lcd.print("abiertas ");
        delay(1000);
      }
    }
  }


  // Limpiar la pantalla
  lcd.clear();

 // Mostrar datos en la LCD
  switch (displayMode ) {
    case 0: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("H:");
        lcd.print(humidity);
        lcd.print("% T:");
        lcd.print(temperatureC);
        lcd.print("C");
        break;
    }
    case 1: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("H:");
        lcd.print(humidity);
        lcd.print("% T:");
        lcd.print(temperatureC);
        lcd.print("C");
        break;
    }
    case 2: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Luz:");
        lcd.print(lightLevel);
        break;
    }
    case 3: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gyro X:");
        lcd.print(gx);
        lcd.setCursor(0, 1);
        lcd.print("Y:");
        lcd.print(gy);
        lcd.print(" Z:");
        lcd.print(gz);
        break;
    }
  }
  
    displayMode = (displayMode + 1) % 5;

  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.print("% Temperatura DHT: ");
  Serial.print(temperatureC);
  Serial.print("C ");
  Serial.print("Luz: ");
  Serial.print(lightLevel);
  Serial.print(pos);


  delay(2000); // Esperar 2 segundos antes de la siguiente lectura
}