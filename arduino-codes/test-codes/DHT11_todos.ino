#include <DHT.h>

#define DHTTYPE DHT11
#define DHT1PIN 4
#define DHT2PIN 5
#define DHT3PIN 6
#define DHT4PIN 7

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);
DHT dht3(DHT3PIN, DHTTYPE);
DHT dht4(DHT4PIN, DHTTYPE);

float h1;
float h2;
float h3;
float h4;

float t1;
float t2;
float t3;
float t4;

float media;

void setup() {
 Serial.begin(9600);
 dht1.begin();
 dht2.begin();
 dht3.begin();
 dht4.begin();
}

void loop() {
 if(Serial.available()) {
   int num = Serial.read(); 
   if(num == '0'){
     leitura();
     prrint();
  }
 }
}

void leitura(){
  h1 = dht1.readHumidity();
  t1 = dht1.readTemperature();
  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();
  h3 = dht3.readHumidity();
  t3 = dht3.readTemperature();
  h4 = dht4.readHumidity();
  t4 = dht4.readTemperature();

  media = ((t1 + t2 + t3 + t4)/4);
}

void prrint(){
  Serial.println("--------------------------");
  Serial.print("Umidade(1): ");
  Serial.print(h1);
  Serial.println("%");
  Serial.print("Temperatura(1): ");
  Serial.print(t1);
  Serial.println("C ");
  Serial.println("");

  Serial.print("Umidade(2): ");
  Serial.print(h2);
  Serial.println("%");
  Serial.print("Temperatura(2): ");
  Serial.print(t2);
  Serial.println("C ");
  Serial.println("");

  Serial.print("Umidade(3): ");
  Serial.print(h3);
  Serial.println("%");
  Serial.print("Temperatura(3): ");
  Serial.print(t3);
  Serial.println("C ");
  Serial.println("");

  Serial.print("Umidade(4): ");
  Serial.print(h4);
  Serial.println("%");
  Serial.print("Temperatura(4): ");
  Serial.print(t4);
  Serial.println("C ");
  Serial.println("");

  Serial.print("Temperatura media: ");
  Serial.print(media);
  Serial.println("C ");
  Serial.println("");
}

