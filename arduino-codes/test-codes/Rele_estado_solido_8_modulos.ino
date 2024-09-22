#define canal1 2
#define canal2 3
#define canal3 4
#define canal4 5
#define canal5 6
#define canal6 7
#define canal7 8
#define canal8 9

void setup() {
  Serial.begin(9600);
  pinMode(canal1, OUTPUT);
  pinMode(canal2, OUTPUT);
  pinMode(canal3, OUTPUT);
  pinMode(canal4, OUTPUT);
  pinMode(canal5, OUTPUT);
  pinMode(canal6, OUTPUT);
  pinMode(canal7, OUTPUT);
  pinMode(canal8, OUTPUT);
  AllOff();
}

void loop() {
  if(Serial.available()) {
   int num = Serial.read(); 
   if(num == '0'){
     AllOn();
  } else if(num == '9'){
    AllOff();
  } else if(num == '1'){
    digitalWrite(canal1, LOW);
  } else if(num == '2'){
    digitalWrite(canal2, LOW);
  } else if(num == '3'){
    digitalWrite(canal3, LOW);
  } else if(num == '4'){
    digitalWrite(canal4, LOW);
  } else if(num == '5'){
    digitalWrite(canal5, LOW);
  } else if(num == '6'){
    digitalWrite(canal6, LOW);
  } else if(num == '7'){
    digitalWrite(canal7, LOW);
  } else if(num == '8'){
    digitalWrite(canal8, LOW);
  }
 }

}

void AllOn(){
  digitalWrite(canal1, LOW);
  digitalWrite(canal2, LOW);
  digitalWrite(canal3, LOW);
  digitalWrite(canal4, LOW);
  digitalWrite(canal5, LOW);
  digitalWrite(canal6, LOW);
  digitalWrite(canal7, LOW);
  digitalWrite(canal8, LOW);
}

void AllOff(){
  digitalWrite(canal1, HIGH);
  digitalWrite(canal2, HIGH);
  digitalWrite(canal3, HIGH);
  digitalWrite(canal4, HIGH);
  digitalWrite(canal5, HIGH);
  digitalWrite(canal6, HIGH);
  digitalWrite(canal7, HIGH);
  digitalWrite(canal8, HIGH);
}

