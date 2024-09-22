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
String storedTags[] = {"e13ee394", "5325f9a3", "6384f9a3", "c303f7a3"};
String storedNames[] = {"Leonardo", "Barbara", "Ana", "Giovana"};
int brilho = 250;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação Serial
  SPI.begin(); // Inicializa comunicação SPI
  mfrc522.PCD_Init(); // Inicializa o leitor RFID
  
  strip.begin(); 
  limpa();
  strip.setBrightness(brilho);
  strip.show();
  LedPadrao(strip.Color(255, 0, 0));
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
      led(strip.Color(0, 255, 0));
      found = true;
      break;
    }
  }
  
  if (!found) {
    Serial.println("Tag not found: " + IDtag);
    led(strip.Color(255, 0, 0));
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  LedPadrao(strip.Color(255, 0, 0));
 }
}

void limpa(){
  for (int j = 0; j < LED_COUNT; j++) {    
      strip.clear();
      strip.show();
  }
}

void led(uint32_t color) {
  for (int k = 0; k < 2; k++){
   int dell = 200;
   for (int j = 0; j < LED_COUNT; j++) {    
      strip.setPixelColor(j, color);
      strip.show();
  }
  delay(dell);
  limpa();
  delay(dell);
 }
}

void LedPadrao(uint32_t color){
  for (int j = 0; j < LED_COUNT; j++) {    
      strip.setPixelColor(j, color);
      strip.show();
  }
}

