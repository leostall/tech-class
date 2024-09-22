#define buzz 38

void setup() {
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) {
    int num = Serial.read(); 
    if(num == '1'){
      digitalWrite(buzz, HIGH);
    } else if(num == '0'){
      digitalWrite(buzz, LOW);     
    } else{
      Serial.print("NUMERO NAO RECONHECIDO: ");
      Serial.println(num);
    }
  }
}
