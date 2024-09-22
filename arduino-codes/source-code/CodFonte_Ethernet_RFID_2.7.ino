/*
  Explicação do código:

    RFID's (todos utilizam o mesmo código PHP): Os módulos fazem a leitura e enviam ao código PHP o IDtag e o módulos que fois lido o cartão. Após
    o código realiza toda a verificação (tanto na tabela 3°INF-C quanto na de colaboradores).

    O Arduino envia os dados exegidos pelo código PHP, e após, retorna os dados (para tivar o ou não os relés). Caso os dados sejam =1, ele ativa o relé;
    caso seja =0, ele não ativa o relé. Para a projeção o arduino recebe os dois códigos (liga/desliga e o de congelamento), caso o dado recebido seja =2
    o arduino liga/desliga o projetor e caso receba =3 ele congela/descongela a imagem.

    Junto a isso, foi inserido o cóigo para enviar a temperatura da sala (pelo sensor 01 e sensor 02); os dados são enviados para o ventilador1 e ventilador2
    respectivamente. 

  Hardwares presentes:

    3 Módulos RFID;
    6 Módulos led's (8 unidades cada);
    3 Buzzers passivos;
    1 Relé de estado sólido de 8 canais;
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

#define ReleVentilador01 22
#define ReleVentilador02 23
#define ReleComputador 24
#define ReleLEDQuadro 25

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN_3);

CRGB leds_1[LED_COUNT];
CRGB leds_2[LED_COUNT];
CRGB leds_3[LED_COUNT];

String mensagem;

String UrlServerExecutePHP_1 = "GET /ArduinoRFID.php?TAG_NFC=";
String UrlServerExecutePHP_2 = "&MODULO=";

String UrlServerSerachPHP_1 = "GET /ArduinoPerifericos.php?valV001=";
String UrlServerSerachPHP_2 = "&valV002=";
String UrlServerSerachPHP_3 = "&statusV001="; 
String UrlServerSerachPHP_4 = "&statusV002=";
String UrlServerSerachPHP_5 = "&statusC001=";
String UrlServerSerachPHP_6 = "&statusP001=";

String StatusVentilador01 = "DESLIGADO";
String StatusVentilador02 = "DESLIGADO";
String StatusProjetor = "DESLIGADO";
String StatusComputador = "DESLIGADO";

String IDtag = "";

String onOff = "";
String freeze = "";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192, 168, 1, 100); //IPV4 (PC - CMD)
IPAddress ip(192, 168, 1, 50); //(ultimo OCTETO ñ pode ser 1 ou 2)
IPAddress myDns(192, 168, 11, 4); 

EthernetClient client;

static uint32_t timer;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;

int modulo;
int loopvalue;
int C001Value;
int P001Value;
int V001Value;
int V002Value;

bool status = false;

float temp01;
float temp02;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();
  mfrc522_3.PCD_Init();

  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds_1, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds_2, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_3>(leds_3, LED_COUNT);
  FastLED.setBrightness(200);

  Serial.println("SISTEMA DESENVOLVIDO POR *TECH CLASS*");
  Serial.println("     TODOS OS DIREITOS RESERVADOS    ");
  Serial.println("     ---  INICIANDO SISTEMA  ---     ");
  Serial.println("");

  delay(700);

  pinMode(buzzer_1, OUTPUT);
  pinMode(buzzer_2, OUTPUT);
  pinMode(buzzer_3, OUTPUT);
  pinMode(ReleVentilador01, OUTPUT);
  pinMode(ReleVentilador02, OUTPUT);
  pinMode(ReleComputador, OUTPUT);
  pinMode(ReleLEDQuadro, OUTPUT);

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
  ChekValuesServer();
}

void loop() {
  delay(1);
  loopvalue = loopvalue + 1;

  LeituraEntrada();
  LeituraSaida(); 
  LeituraPerifericos();

  if(loopvalue == 150){
    ChekValuesServer();
    loopvalue = 0;
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

    ChekServer();

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
    
    ChekServer();

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
    
    ChekServer();

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
    Serial.println("Tag detectada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: entrada");
    Serial.println("");

    for(int i = 0; i < 2; i++){
      fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 0));
      tone(buzzer_1, 1000);
      int k = 16;
      for (int j = 0; j < 8; j++) {
        leds_1[j] = CRGB(0, 250, 0);
        leds_1[k] = CRGB(0, 250, 0);
        FastLED.show();
        k = k - 1;
        delay(80);
      }
      noTone(buzzer_1);
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

void Perifericos(){
  if(V001Value == 1){
    digitalWrite(ReleVentilador01, LOW);
    StatusVentilador01 = "LIGADO";
    Serial.println("VENTILADOR 01 LIGADO");
  }else{
    digitalWrite(ReleVentilador01, HIGH);
    StatusVentilador01 = "DESLIGADO";
    Serial.println("VENTILADOR 01 DESLIGADO");
  }

  if(V002Value == 1){
    digitalWrite(ReleVentilador02, LOW);
    StatusVentilador02 = "LIGADO";
    Serial.println("VENTILADOR 02 LIGADO");
  }else{
    digitalWrite(ReleVentilador02, HIGH);
    StatusVentilador02 = "DESLIGADO";
    Serial.println("VENTILADOR 02 DESLIGADO");
  }

  if(C001Value == 1){
    digitalWrite(ReleComputador, LOW);
    StatusComputador = "LIGADO";
    Serial.println("COMPUTADOR LIGADO");
  }else{
    digitalWrite(ReleComputador, HIGH);
    StatusComputador = "DESLIGADO";
    Serial.println("COMPUTADOR DESLIGADO");
  }

  if(P001Value == 1){
    digitalWrite(ReleLEDQuadro, LOW);
    StatusProjetor = "LIGADO";
    Serial.println("LED LIGADO");
  }else if(P001Value == 2){
    Serial.println("LIGAR/DESLIGAR");
    P001Value = 1;
  }else if(P001Value == 3){
    Serial.println("CONGELAR");
    P001Value = 1;
  }else{
    digitalWrite(ReleLEDQuadro, HIGH);
    StatusProjetor = "DESLIGADO";
    Serial.println("LED DESLIGADO");
  }
}

void ChekValuesServer(){
  mensagem = UrlServerSerachPHP_1 + temp01 + UrlServerSerachPHP_2 + temp02 + UrlServerSerachPHP_3 + StatusVentilador01 + UrlServerSerachPHP_4 + StatusVentilador02 + UrlServerSerachPHP_5 + StatusComputador + UrlServerSerachPHP_6 + StatusProjetor;
  EnviaRequisicao(mensagem);
  Serial.println(mensagem);

  while (client.connected()) {
    if (client.available()) {
      String response = client.readStringUntil('\n');

      Serial.println(response);

      C001Value = getValue(response, "C001:");
      P001Value = getValue(response, "P001:");
      V001Value = getValue(response, "V001:");
      V002Value = getValue(response, "V002:");

      int onOffStart = response.indexOf("ONOFF:");
      if (onOffStart >= 0) {
        int onOffEnd = response.indexOf(";", onOffStart);
        if (onOffEnd >= 0) {
          onOff = response.substring(onOffStart + 6, onOffEnd);
        }
      }

      int freezeStart = response.indexOf("FREEZE:");
      if (freezeStart >= 0) {
        int freezeEnd = response.indexOf(";", freezeStart);
        if (freezeEnd >= 0) {
          freeze = response.substring(freezeStart + 7, freezeEnd);
        }
      }
      
      /*
      Serial.print("C001: ");
      Serial.println(C001Value);
      Serial.print("P001: ");
      Serial.println(P001Value);
      Serial.print("V001: ");
      Serial.println(V001Value);
      Serial.print("V002: ");
      Serial.println(V002Value);
      Serial.print("Cod1 (on/off): ");
      Serial.println(onOff);
      Serial.print("Cod2 (freeze): ");
      Serial.println(freeze);
      */

      Perifericos();
    }
  }
  client.stop();
}

void ChekServer() {
  Serial.println("");
  Serial.println("///// CHEK CODE SERVER /////");
  mensagem = UrlServerExecutePHP_1 + IDtag + UrlServerExecutePHP_2 + modulo;
  EnviaRequisicao(mensagem);
  Serial.println(mensagem);

  while (client.connected()) {
    if (client.available()) {

      int c = client.read();
      Serial.print("RESPOSTA SERVER: ");
      Serial.println(c);

      if (c == 49) {

        status = true;
        Serial.println("CÓDIGO VÁLIDO :)");

      } else { //48
        status = false;
        Serial.println("CÓDIGO INVALIDO OU NÃO CADASTRADO :(");
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
    Serial.println("*** FALHA AO CONECTAR AO SERVIDOR ***");
    
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
  delay(1000);
  Serial.print("conectando a: ");
  Serial.print(server);
  Serial.println("...");
}

int getValue(String data, String id) {
  int start = data.indexOf(id);
  if (start >= 0) {
    start += id.length();
    int end = data.indexOf(';', start);
    if (end >= 0) {
      String value = data.substring(start, end);
      return value.toInt();
    }
  }
  return -1; // Valor de erro se o dado não for encontrado
}