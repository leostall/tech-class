/*
  Explicação do código:

    RFID's (todos utilizam o mesmo código PHP): Os módulos fazem a leitura e enviam ao código PHP o IDtag e o módulos que fois lido o cartão. Após
    o código realiza toda a verificação (tanto na tabela 3°INF-C quanto na de colaboradores).

  Hardwares presentes:

    3 Módulos RFID;
    6 Módulos led's (8 unidades cada);
    3 Buzzers passivos;
*/

#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <FastLED.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

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

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN_3);

CRGB leds_1[LED_COUNT];
CRGB leds_2[LED_COUNT];
CRGB leds_3[LED_COUNT];

String mensagem;

String UrlServerExecutePHP_1 = "GET /arduino.php?TAG_NFC=";
String UrlServerExecutePHP_2 = "&MODULO=";

String IDtag = "";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192, 168, 1, 100); //IPV4 (PC - CMD)
IPAddress ip(192, 168, 1, 50); //(ultimo OCTETO ñ pode ser 1 ou 2)
IPAddress myDns(192, 168, 11, 4); 

char user[] = "root";
char password[] = "";
char db[] = "tecpuc";

EthernetClient client;
MySQL_Connection conn((Client *)&client);

static uint32_t timer;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;

int modulo;
bool status = false;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
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

  teste_temp();
}

void loop() {
  LeituraEntrada();
  LeituraSaida(); 
  LeituraPerifericos();
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
      Serial.println("Ethernet shield não está funcionando. Desculpe, não pode ser executado sem hardware. :(");
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

    Serial.println("Conectando ao banco de dados...");
    if (conn.connect(server, 3306, user, password)) {
      Serial.println("Conexão bem-sucedida!");
    } else {
      Serial.println("Falha na conexão ao banco de dados.");
    }
  }
  delay(1000);
  Serial.print("conectando a: ");
  Serial.print(server);
  Serial.println("...");
}

void teste_temp(){
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  char query[] = "SELECT VAL1 FROM perifericos_3c WHERE ID = 'V001'";
  cur_mem->execute(query);

  float valor_corte_ventilador = cur_mem->get_float(0);
  Serial.print("Valor do ventilador: ");
  Serial.println(valor_corte_ventilador);
  // Faça o processamento necessário com o valor do ventilador

  delete cur_mem;
}
