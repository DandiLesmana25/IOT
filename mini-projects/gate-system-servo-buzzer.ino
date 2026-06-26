#include <Servo.h>

Servo gateServo;

// pin HC-SR04
int trigPin = 7;
int echoPin = 6;
   
// pin servo & buzzer
int servoPin = 9;
int buzzerPin = 8;

// variabel jarak
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  gateServo.attach(servoPin);

  Serial.begin(9600);

  // posisi awal palang (tutup)
  gateServo.write(0);
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
    gateServo.write(90);
    digitalWrite(buzzerPin, HIGH);
  } else {
    // tutup palang
    gateServo.write(0);
    digitalWrite(buzzerPin, LOW);
  }

  delay(200);
}
