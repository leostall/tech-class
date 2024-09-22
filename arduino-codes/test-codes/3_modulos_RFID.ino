/*CÓDIFGO GERADO PELO CHAR GPT, DEVE TRABALHAR COM TRÊS MÓDULOS

Os três módulos RC522 podem ser conectados aos seguintes pinos:

Módulo 1:

SDA: pino 10 (ICSP-4)
SCK: pino 13 (ICSP-3)
MOSI: pino 11 (ICSP-4)
MISO: pino 12 (ICSP-1)
IRQ: não conectado
GND: GND
RST: pino 9
3,3V: 3,3V

Módulo 2:

SDA: pino 22
SCK: pino 23
MOSI: pino 24
MISO: pino 25
IRQ: não conectado
GND: GND
RST: pino 26
3,3V: 3,3V

Módulo 3:

SDA: pino 30
SCK: pino 31
MOSI: pino 32
MISO: pino 33
IRQ: não conectado
GND: GND
RST: pino 34
3,3V: 3,3V
Observe que os pinos SDA, SCK, MOSI, MISO e GND são os mesmos para todos os módulos, 
enquanto que os pinos RST devem ser diferentes para cada módulo. 
É importante também conectar uma alimentação de 3.3V ao módulo, pois a lógica do RC522 funciona com essa tensão.

------------------------------------------------------------------------

O módulo RC522 utiliza os seguintes pinos:

SDA (dados): pode ser conectado a qualquer pino digital. No código fornecido, é utilizado o pino 10.
SCK (relógio): deve ser conectado ao pino 13.
MOSI (saída de dados): deve ser conectado ao pino 11.
MISO (data in): deve ser conectado ao pino 12.
IRQ (solicitação de interrupção): pode ser deixado sem conexão ou conectado a qualquer pino digital.

Os três módulos RC522 podem ser conectados aos pinos mencionados acima, desde que cada um deles tenha um pino diferente para o SDA. 
Por exemplo, se os módulos foram conectados aos pinos 10, 9 e 8, respectivamente, os códigos relacionados aos módulos devem ser atualizados para refletir esses pinos.
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN_1 53 // Pin para o módulo 1
#define RST_PIN_1 9
#define SS_PIN_2 46 // Pin para o módulo 2
#define RST_PIN_2 6
#define SS_PIN_3 42 // Pin para o módulo 3
#define RST_PIN_3 3

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN_3);

String IDtag = "";
String storedTags[] = {"e13ee394", "5325f9a3", "6384f9a3", "c303f7a3"};
String storedNames[] = {"Leonardo", "Barbara", "Ana", "Giovana"};

void setup() {
  Serial.begin(9600); // Inicializa a comunicação Serial
  SPI.begin(); // Inicializa comunicação SPI
  mfrc522_1.PCD_Init(); // Inicializa o leitor RFID do módulo 1
  mfrc522_2.PCD_Init(); // Inicializa o leitor RFID do módulo 2
  mfrc522_3.PCD_Init(); // Inicializa o leitor RFID do módulo 3
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
        found = true;
        break;
      }
    }
    
    if (!found) {
      Serial.println("");
      Serial.println("Tag not found: " + IDtag);
      Serial.println("Modulo: 1");
    }

    mfrc522_1.PICC_HaltA();
    mfrc522_1.PCD_StopCrypto1();
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
        found = true;
        break;
      }
    }
    
    if (!found) {
      Serial.println("");
      Serial.println("Tag not found: " + IDtag);
      Serial.println("Modulo: 2");
    }

    mfrc522_2.PICC_HaltA();
    mfrc522_2.PCD_StopCrypto1();
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
        found = true;
        break;
      }
    }
    
    if (!found) {
      Serial.println("");
      Serial.println("Tag not found: " + IDtag);
      Serial.println("Modulo: 3");
    }

    mfrc522_3.PICC_HaltA();
    mfrc522_3.PCD_StopCrypto1();
  }
}
