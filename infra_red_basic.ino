
// definisi pin 
const int irPin = 2;
const int buzzerPin = 8;
const int  ledPin = 13;



void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:


  //  baca data sensor
  int statusSensor = digitalRead(irPin);
  if (statusSensor == LOW) {
    Serial.println("Awassss!! ada halangan");
     digitalWrite(ledPin, HIGH);
     digitalWrite(buzzerPin, HIGH);
  } else {
    Serial.println("jalanan bersih....");
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  delay(100); //delay 100 milisecond
}
