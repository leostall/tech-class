/*
  Explicação do código:

    RFID's (os dois utilizam o mesmo código PHP): Os módulos fazem a leitura e enviam ao código PHP o IDtag e o módulo que foi lido o cartão. Após
    o código (PHP) realiza toda a verificação (tanto na tabela 3°INF-C quanto na de colaboradores).

    O Arduino envia os dados exegidos pelo código PHP, e após, retorna os dados (para ativar o ou não os relés). Caso os dados sejam =1, ele ativa o relé;
    caso seja =0, ele não ativa o relé. Para a projeção o arduino recebe os dois códigos (liga/desliga e o de congelamento), caso o dado recebido seja =2
    o arduino liga/desliga o projetor e caso receba =3 ele congela/descongela a imagem.

    Junto a isso, foi inserido o cóigo para enviar a temperatura da sala (pelo sensor 01 e sensor 02); os dados são enviados para o ventilador1 e ventilador2
    respectivamente além de pegar o valor de corte. 

  Hardwares presentes:

    2 Módulos RFID;
    4 Módulos led's (8 unidades cada);
    2 Buzzers passivos;
    1 Relé de estado sólido de 8 canais;
    2 Módulos de temperatura de humidade (DHT11)
*/

#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <FastLED.h>
#include <IRremote.h>
#include <dht.h>

#define SS_PIN_1 53
#define RST_PIN_1 8
#define SS_PIN_2 46
#define RST_PIN_2 6

#define LED_PIN_1 17
#define LED_PIN_2 18
#define LED_COUNT 16

#define buzzer_1 14
#define buzzer_2 15

#define releComputador 22
#define releLuz4 23
#define releLuz8 24
#define releVentilador01 25
#define releVentilador02 26
#define releLEDQuadro 27

#define DHTPin01 28
#define DHTPin02 29

#define IR_PIN 37

dht DHT;

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);

CRGB leds_1[LED_COUNT];
CRGB leds_2[LED_COUNT];

String mensagem;

String UrlServerExecutePHP_1 = "GET /ArduinoRFID.php?TAG_NFC=";
String UrlServerExecutePHP_2 = "&MODULO=";

String UrlServerSerachPHP_1 = "GET /ArduinoPerifericos.php?valV001=";
String UrlServerSerachPHP_2 = "&valV002=";

String IDtag = "";

String onOffCode = "0x20DF10EX";
String freezeCode = "0x20DF00FX";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192, 168, 1, 101); //IPV4 (PC - CMD)
IPAddress ip(192, 168, 1, 30); //(ultimo OCTETO ñ pode ser 1 ou 2)
IPAddress myDns(192, 168, 11, 4);

EthernetClient client;

static uint32_t timer;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;

int modulo;
int loopvalue;
int C001Value;
int L004Value;
int L008Value;
int V001Value;
int V002Value;
int P001Value;

bool status = false;

float temp01;
float temp02;

unsigned long onOff = 0;
unsigned long freeze = 0;

IRsend irsend;
decode_results res;
decode_type_t tipo;


void setup() {
  Serial.begin(9600);
  SPI.begin();

  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();

  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds_1, LED_COUNT);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds_2, LED_COUNT);
  FastLED.setBrightness(200);

  Serial.println("SISTEMA DESENVOLVIDO POR *TECH CLASS*");
  Serial.println("     TODOS OS DIREITOS RESERVADOS    ");
  Serial.println("     ---  INICIANDO SISTEMA  ---     ");
  Serial.println("V:1.5");
  Serial.println("");

  delay(700);

  pinMode(buzzer_1, OUTPUT);
  pinMode(buzzer_2, OUTPUT);

  pinMode(releComputador, OUTPUT);
  pinMode(releLuz4, OUTPUT);
  pinMode(releLuz8, OUTPUT);
  pinMode(releVentilador01, OUTPUT);
  pinMode(releVentilador02, OUTPUT);
  pinMode(releLEDQuadro, OUTPUT);

  relesOff();

  fill_solid(leds_1, LED_COUNT, CRGB(0, 0, 250));
  fill_solid(leds_2, LED_COUNT, CRGB(0, 0, 250));
  FastLED.show();

  ConnectServer();

  fill_solid(leds_1, LED_COUNT, CRGB(250, 0, 0));
  fill_solid(leds_2, LED_COUNT, CRGB(250, 0, 0));
  FastLED.show();

  Serial.println("");
  Serial.println("SISTEMA ON-LINE E OPERANDO");
  Serial.println(":)");

  ChekValuesServer();
}

void loop(){
  LeituraEntrada();
  delay(1);
  LeituraSaida(); 
  delay(1);

  if(loopvalue == 150){
    temp01 = DHT.read21(DHTPin01);
    temp02 = DHT.read22(DHTPin02);
    ChekValuesServer();
    loopvalue = 0;
  }else{
    loopvalue = loopvalue + 1;
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

void Access_1(bool access){
  if(access == true){
    Serial.println("Tag detectada no banco de dados: " + IDtag);
    Serial.println("Modulo receptor: entrada");
    Serial.println("");

    /*for(int i = 0; i < 2; i++){
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
    }*/

    for(int i = 0; i < 2; i++){ //400ms parado
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

void Perifericos(){
  if(C001Value == 1){
    digitalWrite(releComputador, LOW);
  }else{
    digitalWrite(releComputador, HIGH);
  }
  
  if(L004Value == 1){
    digitalWrite(releLuz4, LOW);
  }else{
    digitalWrite(releLuz4, HIGH);
  }
  
  if(L008Value == 1){
    digitalWrite(releLuz8, LOW);
  }else{
    digitalWrite(releLuz8, HIGH);
  }
  
  if(V001Value == 1){
    digitalWrite(releVentilador01, LOW);
  }else{
    digitalWrite(releVentilador01, HIGH);
  }
  
  if(V002Value == 1){
    digitalWrite(releVentilador02, LOW);
  }else{
    digitalWrite(releVentilador02, HIGH);
  }
  
  if(P001Value == 2){
    digitalWrite(releLEDQuadro, LOW); //COLOCAR COD HEX DEC. PARA LIGAR 
    Serial.println("LIGANDO PROJETOR");

    irsend.sendNEC(0x20DF10EF, 32); // PROJETOR EPSON - SALA COLÉGIO
    //irsend.sendNEC(0x20DF00FF, 32); // PROJETOR ACER - LEONARDO
  }

  if(P001Value == 3){
    digitalWrite(releLEDQuadro, HIGH); //COLOCAR COD HEX DEC. PARA DESLIGAR
    Serial.println("DESLIGANDO PROJETOR");

    irsend.sendNEC(0x20DF10EF, 32); // PROJETOR EPSON - SALA COLÉGIO
    //irsend.sendNEC(0x20DF00FF, 32); // PROJETOR ACER - LEONARDO
    delay(200);

    irsend.sendNEC(0x20DF10EF, 32); // PROJETOR EPSON - SALA COLÉGIO
    //irsend.sendNEC(0x20DF00FF, 32); // PROJETOR ACER - LEONARDO - 0x10C8E11E
  }
  
  if(P001Value == 4){ //COLOCAR COD HEX DEC. PARA CONGELAR
    Serial.println("COLGELANDO IMAGEM PROJETOR");

    irsend.sendNEC(0x20DF00FF, 32); // PROJETOR EPSON - SALA COLÉGIO
    //irsend.sendNEC(freeze, 32); // PROJETOR ACER - LEONARDO - 0x10C8718E
  }
}

void ChekValuesServer(){
  mensagem = UrlServerSerachPHP_1 + temp01 + UrlServerSerachPHP_2 + temp02;
  EnviaRequisicao(mensagem);
  Serial.println(mensagem);

  while (client.connected()) {
    if (client.available()) {
      String response = client.readStringUntil('\n');

      Serial.println(response);

      C001Value = getValue(response, "C001:");
      L004Value = getValue(response, "L004:");
      L008Value = getValue(response, "L008:");
      V001Value = getValue(response, "V001:");
      V002Value = getValue(response, "V002:");
      P001Value = getValue(response, "P001:");

      unsigned long onOff = 0; // Declare como unsigned long

int onOffStart = response.indexOf("ONOFF:");
if (onOffStart >= 0) {
  int onOffEnd = response.indexOf(";", onOffStart);
  if (onOffEnd >= 0) {
    String onOffString = response.substring(onOffStart + 6, onOffEnd);
    onOff = strtoul(onOffString.c_str(), NULL, 16); // Converte a String hexadecimal em um unsigned long
  }
}



      int freezeStart = response.indexOf("FREEZE:");
      if (freezeStart >= 0) {
        int freezeEnd = response.indexOf(";", freezeStart);
        if (freezeEnd >= 0) {
          freeze = freezeStart + 7, freezeEnd;
        }
      }
      
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

void relesOff(){
  digitalWrite(releComputador, HIGH);
  digitalWrite(releLuz4, HIGH);
  digitalWrite(releLuz8, HIGH);
  digitalWrite(releVentilador01, HIGH);
  digitalWrite(releVentilador02, HIGH);
  digitalWrite(releLEDQuadro, HIGH);
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
  return -1;
}