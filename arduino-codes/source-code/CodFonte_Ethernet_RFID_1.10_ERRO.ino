/*
  Explicação do código:

    RFID (entrada): evia o código NFC para o código PHP, e pós muda o status para PRESENTE. Este retorna se o código está dacastrado no banco de 
    dados (retorna 0 ou 1 (estamos com erro nesta parte, pois envia 48 ou 49)). Este faz a verificação na tabela "3infc" e "colaboradores";

    RFID (saída): evia o código NFC para o código PHP, e pós muda o status para AUSENTE. Este retorna se o código está dacastrado no banco de 
    dados (retorna 0 ou 1 (estamos com erro nesta parte, pois envia 48 ou 49)). Este faz a verificação na tabela "3infc" e "colaboradores";

    RFID (professor): evia o código NFC para o código PHP. Este retorna se o código está dacastrado no banco de dados (retorna 0 ou 1 (estamos 
    com erro nesta parte, pois envia 48 ou 49)). Este só verifica na tabela "colaboradores".

  Hardwares presentes:

    3 Módulos RFID;
    6 Módulos led's (8 unidades cada);
    3 Buzzers passivos;
    1 Botão: 
      Função botão (botao_close): travar sala;
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

#define LED_PIN_1 17
#define LED_PIN_2 18
#define LED_PIN_3 19
#define LED_COUNT 16

#define buzzer_1 14
#define buzzer_2 15
#define buzzer_3 16

#define botao_close 22

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
String UrlUpdatePresente_colaboradores = "GET /update_presente_colaboradores.php?nfc_code=";
String UrlUpdateAusente_colaboradores = "GET /update_ausente_colaboradores.php?nfc_code=";
String UrlRFID_Colaboradores = "GET /search_colaboradores_presente.php?nfc_code=";

String UrlCloseRoom = "GET /close_room_3infc.php?";
String UrlSerach_Inspetor_Zelador = "GET /search_colaboradores_inspetor_zelador.php?tag=";

String IDtag = "";

bool RoomStatus = false;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192, 168, 1, 100); //IPV4 (PC - CMD)
IPAddress ip(192, 168, 1, 58); //(ultimo OCTETO ñ pode ser 1 ou 2)
IPAddress myDns(192, 168, 11, 4); 

EthernetClient client;

static uint32_t timer;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;

int brilho = 200;
int modulo;
int ReturnServerUpdate;
bool status = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(buzzer_1, OUTPUT);
  pinMode(buzzer_2, OUTPUT);
  pinMode(buzzer_3, OUTPUT);
  
  pinMode(botao_close, INPUT);

  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();
  mfrc522_3.PCD_Init();

  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds_1, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds_2, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_3>(leds_3, LED_COUNT);
  FastLED.setBrightness(brilho);

  Serial.println("SISTEMA DESENVOLVIDO POR *TECH CLASS*");
  Serial.println("     TODOS OS DIREITOS RESERVADOS    ");
  Serial.println("     ---  INICIANDO SISTEMA  ---     ");
  Serial.println("0");

  delay(700);

  fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 250));
  fill_solid(leds_2, LED_COUNT, CRGB(0, 0, 250));
  fill_solid(leds_3, LED_COUNT, CRGB(0, 0, 250));
  FastLED.show();

  ConnectServer();

  fill_solid(leds_1, LED_COUNT, CRGB(250, 0, 0));
  fill_solid(leds_2, LED_COUNT, CRGB(250, 0, 0));
  fill_solid(leds_3, LED_COUNT, CRGB(250, 0, 0));
  FastLED.show();

  Serial.println("");
  Serial.println("SISTEMA ON-LINE E OPERANDO");
  Serial.println(":)");
  RoomStatus = true;
}

void loop() {

  if(RoomStatus == true){
    LeituraEntrada();
    LeituraSaida();
    LeituraPerifericos();
  } else{
    fill_solid(leds_1, LED_COUNT, CRGB(250, 0, 0));

    for(int i; i < 500; i++){
      LeituraAberturaSala();
    }

    fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 0));

    for(int i; i < 500; i++){
      LeituraAberturaSala();
    }
  }
}

void LeituraEntrada(){
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
      ChekServerColaboradores(UrlSearch_Colaboradores);
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

void LeituraSaida(){
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
      ChekServerColaboradores(UrlSearch_Colaboradores);
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

void LeituraPerifericos(){
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
    
    ChekServerColaboradores(UrlRFID_Colaboradores);

    if (status == true) {
      Access_3(true);
      SelectActionButton();
    } else{
      Access_3(false);
    }

   mfrc522_3.PICC_HaltA();
   mfrc522_3.PCD_StopCrypto1();
   fill_solid(leds_3, LED_COUNT, CRGB(255, 0, 0));
   FastLED.show();
  }
}

void LeituraAberturaSala(){
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial()) {

    IDtag = "";

    for (byte i = 0; i < mfrc522_1.uid.size; i++) {
      IDtag += String(mfrc522_1.uid.uidByte[i] < 0x10 ? "0" : "");
      IDtag += String(mfrc522_1.uid.uidByte[i], HEX);
    }

    modulo = 0;

    if(status == false){
      ChekServerColaboradores(UrlSerach_Inspetor_Zelador);
    }

    if (status == true) {
      RoomStatus = true;
    } else{
      RoomStatus = false;
      Access_1(false);
    }

   mfrc522_1.PICC_HaltA();
   mfrc522_1.PCD_StopCrypto1();
  }
}

void Access_1(bool access){
  if(access == true){
    Serial.println("Tag detectada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: entrada");
    Serial.println("");

    for(int i = 0; i < 2; i++){
      fill_solid(leds_1, LED_COUNT, CRGB(0, 250, 0));
      FastLED.show();
      tone(buzzer_1, 1000);
      delay(100);
      noTone(buzzer_1);
      delay(100);
    }
  } 
  if(access == false){
    Serial.println("Tag não encontrada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: entrada");
    Serial.println("");
    
    for(int i = 0; i < 3; i++){
      fill_solid(leds_1, LED_COUNT, CRGB(250, 0, 0));
      FastLED.show();
      tone(buzzer_1, 100);
      delay(100);
      fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 0));
      FastLED.show();
      noTone(buzzer_1);
      delay(100);
    }
  }
}

void Access_2(bool access){
  if(access == true){
    Serial.println("Tag detectada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: saida");
    Serial.println("");

    for(int i = 0; i < 2; i++){ //400ms parado
      fill_solid(leds_2, LED_COUNT, CRGB(0, 250, 0));
      FastLED.show();
      tone(buzzer_2, 1000);
      delay(100);
      noTone(buzzer_2);
      delay(100);
    }
  } 
  if(access == false){
    Serial.println("Tag não encontrada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: saida");
    Serial.println("");
    
    for(int i = 0; i < 3; i++){ //600ms parado
      fill_solid(leds_2, LED_COUNT, CRGB(250, 0, 0));
      FastLED.show();
      tone(buzzer_2, 100);
      delay(100);
      fill_solid(leds_2, LED_COUNT, CRGB(0, 0, 0));
      FastLED.show();
      noTone(buzzer_2);
      delay(100);
    }
  }
}

void Access_3(bool access){
  if(access == true){
    Serial.println("Tag detectada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: periféricos");
    Serial.println("");

    for(int i = 0; i < 2; i++){ //400ms parado
      fill_solid(leds_3, LED_COUNT, CRGB(0, 250, 0));
      FastLED.show();
      tone(buzzer_3, 1000);
      delay(100);
      noTone(buzzer_3);
      delay(100);
    }
  } 
  if(access == false){
    Serial.println("Tag não encontrada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: periféricos");
    Serial.println("");
    
    for(int i = 0; i < 3; i++){ //600ms parado
      fill_solid(leds_3, LED_COUNT, CRGB(250, 0, 0));
      FastLED.show();
      tone(buzzer_3, 100);
      delay(100);
      fill_solid(leds_3, LED_COUNT, CRGB(0, 0, 0));
      FastLED.show();
      noTone(buzzer_3);
      delay(100);
    }
  }
}

void SelectActionButton(){
  int j;
  Serial.println("SELECT OPTION");
  while(j < 1000){
    j++;
    delay(1);

    if(digitalRead(botao_close) == HIGH){
      j = 0;
      while(j < 1500){
        delay(1);
        j++;
      

      if (mfrc522_3.PICC_IsNewCardPresent() && mfrc522_3.PICC_ReadCardSerial()) {

        IDtag = "";
    
        for (byte i = 0; i < mfrc522_3.uid.size; i++) {

          IDtag += String(mfrc522_3.uid.uidByte[i] < 0x10 ? "0" : "");
          IDtag += String(mfrc522_3.uid.uidByte[i], HEX);
        }

        fill_solid(leds_3, LED_COUNT, CRGB(250, 250, 0));
        FastLED.show();

        modulo = 2;

        ChekServerColaboradores(UrlRFID_Colaboradores);

        if(status ==  true){
          Serial.println("");
          Serial.println("Executando protocolo para fechamento da sala (esta ação só poderá ser desfeita por um inspetor ou zelador");

          EnviaRequisicao(UrlCloseRoom);
          ReturnServerUpdate = client.read();

          if(ReturnServerUpdate == 49){
            Serial.println("");
            Serial.println("PROTOCOLO EXECUTADO COM SUCESSO");
            Serial.println("");

            RoomStatus = false;
          } else{
            Serial.println("ERRO AO EXECUTAR FECHAMNETO DA SALA");
          }
        }
        mfrc522_3.PICC_HaltA();
        mfrc522_3.PCD_StopCrypto1();
        j = 1001;
      }
      }
    }
  }
}



// *** SERVER ***

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
        Serial.println(" - Codigo NFC invalido");
        status = false;
      }
      break;
    }
  }
  client.stop();
}

void ChekServerColaboradores(String Url) {  
  Serial.println("");
  Serial.println("///// CHEK CODE SERVER COLABORADORES /////");
  mensagem = Url + IDtag;
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
          UpdateStatusServer(UrlUpdatePresente_colaboradores);
          Serial.println(ReturnServerUpdate);
        } else if(modulo == 2){
          UpdateStatusServer(UrlUpdateAusente_colaboradores);
          Serial.println(ReturnServerUpdate);
        }
        
        status = true;

        if(ReturnServerUpdate == 49){
          status = true;
        } else{
          status = false;
        }

      } else { //48
        Serial.println(" - Codigo NFC invalido");
        status = false;
      }
      break;
    }
  }
  client.stop();
}

void UpdateStatusServer(String mensagem3){
  Serial.println("");
  Serial.println("///// UPDATE STATUS SERVER /////");
  mensagem2 = mensagem3 + IDtag;
  EnviaRequisicao(mensagem2);
  Serial.println(mensagem2);

  ReturnServerUpdate = client.read();
}

void EnviaRequisicao(String mensagem){
  if (client.connect(server, 80)) { //APACHE usa 80
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
  Serial.println("Inicializando Ethernet com DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha na configuração Ethernet usando DHCP");
    if(Ethernet.hardwareStatus() == EthernetNoHardware){
      Serial.println("Ethernet shield não está funcionando.  Desculpe, não pode ser executado sem hardware. :(");
      while (true) {
        delay(1);
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Cabo de Ethernet não está conectado.");
    }
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP atribuído IP ");
    Serial.println(Ethernet.localIP());
  }
  delay(3000);
  Serial.print("conectando a: ");
  Serial.print(server);
  Serial.println("...");
}