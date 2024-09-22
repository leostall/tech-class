/*
  Explicação do código:

    RFID (entrada): evia o código NFC para o código PHP, e pós muda o status para PRESENTE. Este retorna de o código está dacastrado no banco de 
    dados (retorna 0 ou 1 (estamos com erro nesta parte, pois envia 48 ou 49)). Este faz a verificação na tabela "3infc" e "colaboradores";

    RFID (saída): evia o código NFC para o código PHP, e pós muda o status para AUSENTE. Este retorna de o código está dacastrado no banco de 
    dados (retorna 0 ou 1 (estamos com erro nesta parte, pois envia 48 ou 49)). Este faz a verificação na tabela "3infc" e "colaboradores";

    RFID (professor): evia o código NFC para o código PHP. Este retorna de o código está dacastrado no banco de dados (retorna 0 ou 1 (estamos 
    com erro nesta parte, pois envia 48 ou 49)). Este só verifica na tabela "colaboradores".

  Hardwares presentes:

    3 Módulos RFID;
    6 Módulos led's (8 unidades cada);
    3 Buzzers passivos;
*/
#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <FastLED.h>

#define SS_PIN_1 53
#define RST_PIN_1 8
#define SS_PIN_2 46
#define RST_PIN_2 6
#define SS_PIN_3 42
#define RST_PIN_3 3

#define LED_PIN_1 20
#define LED_PIN_2 19
#define LED_PIN_3 18
#define LED_COUNT 16

#define buzz 49

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN_3);

CRGB leds_1[LED_COUNT];
CRGB leds_2[LED_COUNT];
CRGB leds_3[LED_COUNT];

String mensagem;
String mensagem2;
String UrlSearch_3infc = "GET /search_3infc.php?tag=";
String UrlUpdatePresente_3infc = "GET /update_presente_3infc.php?nfc_code=";
String UrlUpdateAusente_3infc = "GET /update_ausente_3infc.php?nfc_code=";

String UrlSearch_Colaboradores = "GET /search_colaboradores.php?tag=";
String UrlUpdate_Colaboradores = "GET /update_colaboradores.php?nfc_code=";

String IDtag = "";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192, 168, 1, 15); //IPV4 (PC - CMD)
IPAddress ip(192, 168, 1, 50); //(ultimo OCTETO ñ pode ser 1 ou 2)
IPAddress myDns(192, 168, 11, 4); 

EthernetClient client;

static uint32_t timer;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

int brilho = 200;
int modulo;
int ReturnServerUpdate;
bool status = false;

void setup() {
  pinMode(buzz, OUTPUT);

  Serial.begin(9600);
  SPI.begin();
  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();
  mfrc522_3.PCD_Init();
  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds_1, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds_2, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_3>(leds_3, LED_COUNT);
  FastLED.setBrightness(brilho);
  fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 250));
  fill_solid(leds_2, LED_COUNT, CRGB(0, 0, 250));
  fill_solid(leds_3, LED_COUNT, CRGB(0, 0, 250));
  FastLED.show();
  ConnectServer();
  fill_solid(leds_1, LED_COUNT, CRGB(250, 0, 0));
  fill_solid(leds_2, LED_COUNT, CRGB(250, 0, 0));
  fill_solid(leds_3, LED_COUNT, CRGB(250, 0, 0));
  FastLED.show();
  Serial.println("PRONTO");
}

void loop() {
  LeituraTags1(); // ENTRADA
  LeituraTags2(); // SAÍDA
  LeituraTags3(); // PROFESSOR
}

void LeituraTags1(){
  status = false;

  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial()) {
    IDtag = "";
    modulo = 1;

    for (byte i = 0; i < mfrc522_1.uid.size; i++) {
      IDtag += String(mfrc522_1.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_1.uid.uidByte[i], HEX);
    }
    fill_solid(leds_1, LED_COUNT, CRGB(250, 250, 0));
    FastLED.show();

    ChekServerAlunos();

    if(status == false){
      ChekServerColaboradores(true);
    }

    if (status == true) {
      Access_1(true);
    } else{
      Access_1(false);
    }

   mfrc522_1.PICC_HaltA();
   mfrc522_1.PCD_StopCrypto1();
   fill_solid(leds_1, LED_COUNT, CRGB(255, 0, 0));
   FastLED.show();
  }
}

void LeituraTags2(){
  status = false;
  modulo = 2;
  if (mfrc522_2.PICC_IsNewCardPresent() && mfrc522_2.PICC_ReadCardSerial()) {
    IDtag = "";
    
    for (byte i = 0; i < mfrc522_2.uid.size; i++) {
      IDtag += String(mfrc522_2.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_2.uid.uidByte[i], HEX);
    }
    fill_solid(leds_2, LED_COUNT, CRGB(250, 250, 0));
    FastLED.show();
    
    ChekServerAlunos();

    if(status == false){
      ChekServerColaboradores(true);
    }

    if (status == true) {
      Access_2(true);
    } else{
      Access_2(false);
    }

   mfrc522_2.PICC_HaltA();
   mfrc522_2.PCD_StopCrypto1();
   fill_solid(leds_2, LED_COUNT, CRGB(255, 0, 0));
   FastLED.show();
  }
}

void LeituraTags3(){
  status = false;
  modulo = 3;
  if (mfrc522_3.PICC_IsNewCardPresent() && mfrc522_3.PICC_ReadCardSerial()) {
    IDtag = "";
    
    for (byte i = 0; i < mfrc522_3.uid.size; i++) {
      IDtag += String(mfrc522_3.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_3.uid.uidByte[i], HEX);
    }

    fill_solid(leds_3, LED_COUNT, CRGB(250, 250, 0));
    FastLED.show();
    
    ChekServerColaboradores(false);

    if (status == true) {
      Access_3(true);
    } else{
      Access_3(false);
    }

   mfrc522_3.PICC_HaltA();
   mfrc522_3.PCD_StopCrypto1();
   fill_solid(leds_3, LED_COUNT, CRGB(255, 0, 0));
   FastLED.show();
  }
}

void Access_1(bool access){
  if(access == true){
    Serial.println("Tag found: " + IDtag);
    Serial.println("Modulo: entrada");
    Serial.println("");

    for(int i = 0; i < 2; i++){
      fill_solid(leds_1, LED_COUNT, CRGB(0, 250, 0));
      FastLED.show();
      tone(buzz, 1000);
      delay(100);
      noTone(buzz);
      delay(100);
    }
  } 
  if(access == false){
    Serial.println("Tag not found: " + IDtag);
    Serial.println("Modulo: entrada");
    Serial.println("");
    
    for(int i = 0; i < 3; i++){
      fill_solid(leds_1, LED_COUNT, CRGB(250, 0, 0));
      FastLED.show();
      tone(buzz, 100);
      delay(100);
      fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 0));
      FastLED.show();
      noTone(buzz);
      delay(100);
    }
  }
}

void Access_2(bool access){
  if(access == true){
    Serial.println("Tag found: " + IDtag);
    Serial.println("Modulo: saida");
    Serial.println("");

    for(int i = 0; i < 2; i++){ //400ms parado
      fill_solid(leds_2, LED_COUNT, CRGB(0, 250, 0));
      FastLED.show();
      tone(buzz, 1000);
      delay(100);
      noTone(buzz);
      delay(100);
    }
  } 
  if(access == false){
    Serial.println("Tag not found: " + IDtag);
    Serial.println("Modulo: saida");
    Serial.println("");
    
    for(int i = 0; i < 3; i++){ //600ms parado
      fill_solid(leds_2, LED_COUNT, CRGB(250, 0, 0));
      FastLED.show();
      tone(buzz, 100);
      delay(100);
      fill_solid(leds_2, LED_COUNT, CRGB(0, 0, 0));
      FastLED.show();
      noTone(buzz);
      delay(100);
    }
  }
}

void Access_3(bool access){
  if(access == true){
    Serial.println("Tag found: " + IDtag);
    Serial.println("Modulo: professor");
    Serial.println("");

    for(int i = 0; i < 2; i++){
      fill_solid(leds_3, LED_COUNT, CRGB(0, 250, 0));
      FastLED.show();
      tone(buzz, 1000);
      delay(100);
      noTone(buzz);
      delay(100);
    }
  } 
  if(access == false){
    Serial.println("Tag not found: " + IDtag);
    Serial.println("Modulo: professor");
    Serial.println("");
    
    for(int i = 0; i < 3; i++){
      fill_solid(leds_3, LED_COUNT, CRGB(250, 0, 0));
      FastLED.show();
      tone(buzz, 100);
      delay(100);
      fill_solid(leds_3, LED_COUNT, CRGB(0, 0, 0));
      FastLED.show();
      noTone(buzz);
      delay(100);
    }
  }
}

void ChekServerAlunos() {  
  Serial.println("");
  Serial.println("///// CHEK CODE SERVER ALUNOS /////");
  mensagem = UrlSearch_3infc + IDtag;
  EnviaRequisicao(mensagem);
  Serial.println(mensagem);

  // Verifique se a resposta do servidor contém a palavra "valido"
  while (client.connected()) {
    if (client.available()) {
      int c = client.read();
      Serial.print(c);
      if (c == 49) { // utiliza a variável msg para verificar o valor retornado pelo servidor
        Serial.println(" - Codigo NFC valido");

        if(modulo == 1){
          UpdateStatusServer(UrlUpdatePresente_3infc);
          Serial.println(ReturnServerUpdate);
        } else if(modulo == 2){
          UpdateStatusServer(UrlUpdateAusente_3infc);
          Serial.println(ReturnServerUpdate);
        }

        if(ReturnServerUpdate == 49){
          status = true;
        } else{
          status = false;
        }

        
      } else { //48
        Serial.println("Codigo NFC invalido");
        status = false;
      }
      break;
    }
  }
  client.stop();
}

void UpdateStatusServer(String mensagem3){
  Serial.println("");
  Serial.println("///// UPDATE STATUS SERVER ALUNOS /////");
  mensagem2 = mensagem3 + IDtag;
  EnviaRequisicao(mensagem2);
  Serial.println(mensagem2);
  ReturnServerUpdate = client.read();
}

void ChekServerColaboradores(bool stts) {  
  mensagem = UrlSearch_Colaboradores + IDtag;
  EnviaRequisicao(mensagem);
  Serial.println(mensagem);

  // Verifique se a resposta do servidor contém a palavra "valido"
  while (client.connected()) {
    if (client.available()) {
      int c = client.read();
      Serial.println(c);
      if (c == 49) { // utiliza a variável msg para verificar o valor retornado pelo servidor
        Serial.println("Codigo NFC valido");
        if(stts == true){
          mensagem2 = UrlUpdate_Colaboradores + IDtag;
          EnviaRequisicao(mensagem2);
          Serial.println(mensagem2);
        }
        status = true;
      } else { //48
        Serial.println("Codigo NFC invalido");
        status = false;
      }
      break;
    }
  }
  client.stop();
}

void EnviaRequisicao(String mensagem){
  if (client.connect(server, 80)) { //XAMP ou APACHE usa 80
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    client.println(mensagem);
  } else {
    Serial.println("connection failed");
  }
  beginMicros = micros();
}  

void ConnectServer(){
  while (!Serial) {
    ;
  }
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if(Ethernet.hardwareStatus() == EthernetNoHardware){
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1);
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");
}