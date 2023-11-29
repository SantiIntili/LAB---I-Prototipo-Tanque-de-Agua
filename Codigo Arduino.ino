#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 7
#define DHTTYPE DHT11
#define trigPin 8
#define echoPin 9
#define relePin 13
#define relePin2 6

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial BTSerial(11, 10); 

void setup() {
  Serial.begin(9600);
  BTSerial.begin(115200);
  dht.begin();
  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  pinMode(relePin, OUTPUT);
  pinMode(relePin2, OUTPUT);
  digitalWrite(relePin, LOW); 
}

void loop() {
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();
  long duracion;
  float distancia;
  const float distancia_maxima = 23.4;
  const float distancia_minima = 2;
  float porcentaje;
  int inByte = 0; 
  
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH);
  distancia = (duracion / 58.2);
  porcentaje = 100 - ((distancia - distancia_minima) / (distancia_maxima - distancia_minima)) * 100;

  if (BTSerial.available()) {
    while (BTSerial.available()) {
      char receivedChar = BTSerial.read();
      if (receivedChar == '1') {
        digitalWrite(relePin, HIGH);
        delay(5000);
      } else {
        digitalWrite(relePin, LOW);
      }
      if (receivedChar == '2') {
        digitalWrite(relePin2, HIGH);
        delay(5000);
      } else {
        digitalWrite(relePin2, LOW);
      }
    }
  }

  if (porcentaje <= 20) {
    digitalWrite(relePin, HIGH);
    delay(3000);
    digitalWrite(relePin2, LOW);
  } else if (porcentaje >= 35) {
    digitalWrite(relePin, LOW);
    digitalWrite(relePin2, HIGH);
    delay(5000);
  }

  BTSerial.print(porcentaje);
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

  delay(1500);
}
