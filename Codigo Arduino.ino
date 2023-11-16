#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial BTSerial(11, 10); 

const int trigPin = 8; 
const int echoPin = 9; 
const int relePin = 13; 

void setup() {
  Serial.begin(9600);
  BTSerial.begin(115200);
  dht.begin();
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW); 
}

void loop() {
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();
  long duration;
  float distancia;
  int inByte = 0; 

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distancia = (duration / 2) / 29.1;

  if (BTSerial.available()) {
    while (BTSerial.available()) {
      char receivedChar = BTSerial.read();
      if (receivedChar == '1') {
        digitalWrite(relePin, HIGH);
        delay(5000);
      } else {
        digitalWrite(relePin, LOW);
      }
    }
  }

  if (distancia < 15) {
    digitalWrite(relePin, HIGH);
  } else {
    digitalWrite(relePin, LOW);
  }

  BTSerial.print(distancia);
  BTSerial.print(";");
  BTSerial.print(temperatura);
  BTSerial.print(";");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print("°C, Humedad: ");
  Serial.print(humedad);
  Serial.print("%, Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(2000);
}