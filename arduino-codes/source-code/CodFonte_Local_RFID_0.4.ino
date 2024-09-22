#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define SS_PIN_1 53 
#define RST_PIN_1 9
#define SS_PIN_2 46
#define RST_PIN_2 6
#define SS_PIN_3 42
#define RST_PIN_3 3

#define LED_PIN_1 43
#define LED_PIN_2 48
#define LED_PIN_3 4
#define LED_COUNT 8 

#define buzzer 49

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN_3);

Adafruit_NeoPixel strip_1(LED_COUNT, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2(LED_COUNT, LED_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_3(LED_COUNT, LED_PIN_3, NEO_GRB + NEO_KHZ800);

String IDtag = "";
String storedTags[] = {"e13ee394", "5325f9a3", "6384f9a3", "c303f7a3"};
String storedNames[] = {"Leonardo", "Barbara", "Ana", "Giovana"};

int brilho = 250;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação Serial
  SPI.begin(); // Inicializa comunicação SPI
  mfrc522_1.PCD_Init(); // Inicializa o leitor RFID do módulo 1
  mfrc522_2.PCD_Init(); // Inicializa o leitor RFID do módulo 2
  mfrc522_3.PCD_Init(); // Inicializa o leitor RFID do módulo 3

  strip_1.begin();
  strip_2.begin();
  strip_3.begin();

  pinMode(buzzer, OUTPUT);

  Serial.println("INICIANDO");
  strip_1.setBrightness(brilho);
  strip_2.setBrightness(brilho);
  strip_3.setBrightness(brilho);
  strip_1.fill(255, 0, 0);
  strip_2.fill(255, 0, 0);
  strip_3.fill(255, 0, 0);
  strip_1.show();
  strip_2.show();
  strip_3.show();
  Serial.println("PRONTO");
}

void loop(){
  LeituraTags1();
  LeituraTags2();
  LeituraTags3();
}

void LeituraTags1(){
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial()) {
    IDtag = "";
    for (byte i = 0; i < mfrc522_1.uid.size; i++) {
      IDtag += String(mfrc522_1.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_1.uid.uidByte[i], HEX);
    }
  
    bool found = false;
    for (int i = 0; i < sizeof(storedTags) / sizeof(storedTags[0]); i++) {
      if (IDtag.startsWith(storedTags[i])) {
        Serial.println("");
        Serial.println("Tag found: " + IDtag);
        Serial.println("Name: " + storedNames[i]);
        Serial.println("Modulo: 1");
        Piscar_Strip_1(strip_1.Color(0, 255, 0), 2, 200, false);
        found = true;
        break;
      }
    }
    
    if (!found) {
      Serial.println("");
      Serial.println("Tag not found: " + IDtag);
      Serial.println("Modulo: 1");
      Piscar_Strip_1(strip_1.Color(0, 255, 0), 2, 200, true);
    }

    mfrc522_1.PICC_HaltA();
    mfrc522_1.PCD_StopCrypto1();
    strip_1.fill(255, 0, 0);
    strip_1.show();
    
  }
}

void LeituraTags2(){
  if (mfrc522_2.PICC_IsNewCardPresent() && mfrc522_2.PICC_ReadCardSerial()) {
    IDtag = "";
    for (byte i = 0; i < mfrc522_2.uid.size; i++) {
      IDtag += String(mfrc522_2.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_2.uid.uidByte[i], HEX);
    }
  
    bool found = false;
    for (int i = 0; i < sizeof(storedTags) / sizeof(storedTags[0]); i++) {
      if (IDtag.startsWith(storedTags[i])) {
        Serial.println("");
        Serial.println("Tag found: " + IDtag);
        Serial.println("Name: " + storedNames[i]);
        Serial.println("Modulo: 2");
        Piscar_Strip_2(strip_2.Color(0, 255, 0), 2, 200, false);
        found = true;
        break;
      }
    }
    
    if (!found) {
      Serial.println("");
      Serial.println("Tag not found: " + IDtag);
      Serial.println("Modulo: 2");
      Piscar_Strip_2(strip_2.Color(0, 255, 0), 2, 200, true);
    }

    mfrc522_2.PICC_HaltA();
    mfrc522_2.PCD_StopCrypto1();
    strip_2.fill(255, 0, 0);
    strip_2.show();
  }
}

void LeituraTags3(){
  if (mfrc522_3.PICC_IsNewCardPresent() && mfrc522_3.PICC_ReadCardSerial()) {
    IDtag = "";
    for (byte i = 0; i < mfrc522_3.uid.size; i++) {
      IDtag += String(mfrc522_3.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_3.uid.uidByte[i], HEX);
    }
  
    bool found = false;
    for (int i = 0; i < sizeof(storedTags) / sizeof(storedTags[0]); i++) {
      if (IDtag.startsWith(storedTags[i])) {
        Serial.println("");
        Serial.println("Tag found: " + IDtag);
        Serial.println("Name: " + storedNames[i]);
        Serial.println("Modulo: 3");
        Piscar_Strip_3(strip_3.Color(0, 255, 0), 2, 200, false);
        found = true;
        break;
      }
    }
    
    if (!found) {
      Serial.println("");
      Serial.println("Tag not found: " + IDtag);
      Serial.println("Modulo: 3");
      Piscar_Strip_3(strip_3.Color(0, 255, 0), 2, 200, true);
    }

    mfrc522_3.PICC_HaltA();
    mfrc522_3.PCD_StopCrypto1();
    strip_3.fill(255, 0, 0);
    strip_3.show();
  }
}

void Piscar_Strip_1(uint32_t color, int vezes, int dell, bool buzz){
  for (int k = 0; k < vezes; k++){
   if(buzz == true){
    digitalWrite(buzzer, HIGH);
   }
   strip_1.fill(color);
   strip_1.show();
   delay(dell);
   digitalWrite(buzzer, LOW);
   strip_1.clear();
 }
}

void Piscar_Strip_2(uint32_t color, int vezes, int dell, bool buzz){
  for (int k = 0; k < vezes; k++){
   if(buzz == true){
    digitalWrite(buzzer, HIGH);
   }
   strip_2.fill(color);
   strip_2.show();
   delay(dell);
   digitalWrite(buzzer, LOW);
   strip_2.clear();
 }
}

void Piscar_Strip_3(uint32_t color, int vezes, int dell, bool buzz){
  for (int k = 0; k < vezes; k++){
   if(buzz == true){
    digitalWrite(buzzer, HIGH);
   }
   strip_3.fill(color);
   strip_3.show();
   delay(dell);
   digitalWrite(buzzer, LOW);
   strip_3.clear();
 }
}
