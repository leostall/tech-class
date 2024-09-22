#include <Adafruit_NeoPixel.h>

#define LED_PIN 22 //PINO DO LED
#define LED_COUNT 8 //NÚMERO DE LED'S (CASO ADICIONE MAIS MÓDULO DEVERÁ ALTERAR O VALOR)

int brilho = 250; //DEFINE O BRILHO DOS LED'S (ESSE VALOR VAI DE 1 Á 255)

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); //DEFINIÇÃO DA BLIBLIOTECA

void setup() {
  Serial.begin(9600); // INICIA A SERIAL
  strip.begin(); // INICIA OS LED'S
  limpa(); //PAGAR TODOS OS LED'S
  strip.setBrightness(brilho); // DEFINE O BRILHO COMO O INT BRILHO
  strip.show(); //MOSTRA O QUE FOI FEITO NOS LED'S (NO CASO APAGOU E DEFINIU O BRILHO)
}

void loop() {
  serial();
}
void serial(){
    if (Serial.available()) {
    String txt = Serial.readString(); //LÊ O TEXTO DA SERIAL
    if(txt == "teste vermelho"){
      Serial.println("Executando o teste na cor: VERMELHA");
      teste(strip.Color(255, 0, 0)); // TESTE, DEFINIDO A COR VERMELHA
    } else if(txt == "teste verde"){
      Serial.println("Executando o teste na cor: VERDE");
      teste(strip.Color(0, 255, 0)); // TESTE, DEFINIDO A COR VERDE
    } else if(txt == "teste azul"){
      Serial.println("Executando o teste na cor: AZUL");
      teste(strip.Color(0, 0, 255)); // TESTE, DEFINIDO A COR AZUL 
    } else if(txt == "teste branco"){
      Serial.println("Executando o teste na cor: BRANCA(RGB)");
      teste(strip.Color(255, 255, 255)); // TESTE, DEFINIDO A COR BRANCA (RGB)
    } else if(txt == "teste alarme vermelho"){
      Serial.println("Executando o teste do alarme na cor: VERMELHA");
      alarme(strip.Color(255, 0, 0)); // TESTE, ALARME NA COR VERMELHA
    } else{
      Serial.print("TEXTO NAO RECONHECIDO: ");
      Serial.println(txt);
    }
  }
}

void teste(uint32_t color){
  for (int j = 0; j < LED_COUNT; j++) {    
    strip.setPixelColor(j, color);
    strip.show();
    delay(500);
    strip.clear();
    strip.show();
  }
  for (int j = 0; j < LED_COUNT; j++) {    
    strip.setPixelColor(j, color);
  }
  strip.show();
  delay(3000);
  limpa();
}

void alarme(uint32_t color) {
  for (int k = 0; k < 10; k++){ // K É O NÚMERO DE VEZES QUE ELE IRÁ PISCAR
   int dell = 250;
   for (int j = 0; j < LED_COUNT; j++) {    
      strip.setPixelColor(j, color);
      strip.show();
  }
  delay(dell);
  limpa();
  delay(dell);
 }
}

void limpa(){
  for (int j = 0; j < LED_COUNT; j++) {    
      strip.clear();
      strip.show();
  }
}
