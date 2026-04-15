

// pin HC-SR04
int trigPin = 7;
int echoPin = 6;
   
// pin  buzzer
int buzzerPin = 8;

// variabel jarak
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);


  Serial.begin(9600);

}

void loop() {
  // trigger HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // baca echo
  duration = pulseIn(echoPin, HIGH);

  // hitung jarak (cm)
  distance = duration * 0.034 / 2;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 6 && distance > 0) {
    // buka palang
    digitalWrite(buzzerPin, HIGH);
  } else {
    // tutup palang
    digitalWrite(buzzerPin, LOW);
  }

  delay(200);
}
