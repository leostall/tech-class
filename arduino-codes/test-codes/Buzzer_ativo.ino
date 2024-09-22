#define buzz 49

void setup() {
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) {
    int num = Serial.read(); 2
    if(num == '1'){
      tone(buzz, 400, 500); //PERMITIDO
    } else if(num == '2'){
      tone(buzz, 1000, 300); //NEGADO    
    } else if(num == 0){
      noTone(buzz);
    }else{
      Serial.print("NUMERO NAO RECONHECIDO: ");
      Serial.println(num);
    }
  }
}
