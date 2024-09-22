#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define SS_PIN 9
#define RST_PIN 8
#define LED_PIN 22
#define LED_COUNT 8 

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

String IDtag = "";
String storedTags[] = {"e13ee394", "5325f9a3"};
String storedNames[] = {"Leonardo", "Barbara"};
int brilho = 250;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação Serial
  SPI.begin(); // Inicializa comunicação SPI
  mfrc522.PCD_Init(); // Inicializa o leitor RFID
  strip.begin();
  Serial.println("INICIADO");
  LedPiscar(strip.Color(255, 255, 255), 15, 200);
  delay(500); 
  strip.setBrightness(brilho);
  strip.show();
  LedPadrao(strip.Color(255, 0, 0));
  Serial.println("PRONTO");
}

void loop(){
  LeituraTags();
 }

void LeituraTags(){

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    IDtag = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      IDtag += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522.uid.uidByte[i], HEX);
  }
  
  bool found = false;
  for (int i = 0; i < sizeof(storedTags) / sizeof(storedTags[0]); i++) {
    if (IDtag.startsWith(storedTags[i])) {
      Serial.println("Tag found: " + IDtag);
      Serial.println("Name: " + storedNames[i]);
      LedPiscar(strip.Color(0, 255, 0), 2, 200);
      found = true;
      break;
    }
  }
  
  if (!found) {
    Serial.println("Tag not found: " + IDtag);
    LedPiscar(strip.Color(255, 0, 0), 5, 100);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  LedPadrao(strip.Color(255, 0, 0));
 }
}

void LedPiscar(uint32_t color, int vezes, int dell) {
  for (int k = 0; k < vezes; k++){
   strip.fill(color);
   strip.show();
   delay(dell);
   strip.clear();
   strip.show();
   delay(dell);
 }
}

void LedPadrao(uint32_t color){
  for (int j = 0; j < LED_COUNT; j++) {    
      strip.setPixelColor(j, color);
      strip.show();
  }
}
