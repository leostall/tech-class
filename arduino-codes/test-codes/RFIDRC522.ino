#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 9
#define RST_PIN 8

String IDtag = "";
String storedTags[] = {"e13ee394", "5325f9a3", "6384f9a3", "c303f7a3"}; //TAGS
String storedNames[] = {"Leonardo", "Barbara", "Ana", "Giovana"}; //NOMES RESPECTIVOS AS TAGS

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); // Inicializa a comunicação Serial
  SPI.begin(); // Inicializa comunicação SPI
  mfrc522.PCD_Init(); // Inicializa o leitor RFID
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
      found = true;
      break;
    }
  }
  
  if (!found) {
    Serial.println("Tag not found: " + IDtag);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
 }
}


