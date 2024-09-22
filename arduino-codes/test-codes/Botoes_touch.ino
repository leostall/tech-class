#define botao1 13
#define botao2 12
#define botao3 11
#define botao4 10
#define botao5 9
#define botao6 8
#define botao7 7
#define botao8 6
#define botao9 5
#define botao10 4

void setup() {
  Serial.begin(9600);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);
  pinMode(botao5, INPUT);
  pinMode(botao6, INPUT);
  pinMode(botao7, INPUT);
  pinMode(botao8, INPUT);
  pinMode(botao9, INPUT);
  pinMode(botao10, INPUT);
}

void loop() {
  LeituraBotoes();
}

void LeituraBotoes(){
  if(digitalRead(botao1) == HIGH){
    Serial.println("");
    Serial.println("Botão 1 pressionado");
    delay(200);
  } else if(digitalRead(botao2) == HIGH){
    Serial.println("");
    Serial.println("Botão 2 pressionado");
    delay(200);
  } else if(digitalRead(botao3) == HIGH){
    Serial.println("");
    Serial.println("Botão 3 pressionado");
    delay(200);
  } else if(digitalRead(botao4) == HIGH){
    Serial.println("");
    Serial.println("Botão 4 pressionado");
    delay(200);
  } else if(digitalRead(botao5) == HIGH){
    Serial.println("");
    Serial.println("Botão 5 pressionado");
    delay(200);
  } else if(digitalRead(botao6) == HIGH){
    Serial.println("");
    Serial.println("Botão 6 pressionado");
    delay(200);
  } else if(digitalRead(botao7) == HIGH){
    Serial.println("");
    Serial.println("Botão 7 pressionado");
    delay(200);
  } else if(digitalRead(botao8) == HIGH){
    Serial.println("");
    Serial.println("Botão 8 pressionado");
    delay(200);
  } else if(digitalRead(botao9) == HIGH){
    Serial.println("");
    Serial.println("Botão 9 pressionado");
    delay(200);
  } else if(digitalRead(botao10) == HIGH){
    Serial.println("");
    Serial.println("Botão 10 pressionado");
    delay(200);
  }
}