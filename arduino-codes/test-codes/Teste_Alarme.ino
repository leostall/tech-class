#define buzzerPin 49

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Produz um som de alarme em loop
  for(int i = 0; i < 10; i++) {
    tone(buzzerPin, 1000, 200); // Frequência de 1000Hz por 200ms
    delay(200);
    noTone(buzzerPin);
    delay(200);
  }
  delay(2000);
}
