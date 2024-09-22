/*
  LIGAÇÃO DEVE SER FEITA IGUAL A DE BOTÃO

  NOMENCLATURA:
  C = COMUM (LEITURA)
  NO = NORMALMENTE ABERTO (SO DA CONTATO QUANDO "CLICA")
  NC = NORAMLMENTE FECHADO (SO DA CONTATO QEUANDO NÃO ESTA PRESSIONADO)
*/

int val;

void setup() {
  Serial.begin(9600);
  pinMode(10, INPUT);
}

void loop() {
  val = digitalRead(10);
  if(val == 0){
    Serial.println("LOUSA ABERTA");
  }
  else{
    Serial.println("LOUSA FECHADA");
  }
  delay(200);
}
