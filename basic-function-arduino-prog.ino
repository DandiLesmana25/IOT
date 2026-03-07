#include <Servo.h>  // Untuk servo

int mq2Pin = A0;
int batasAsap = 300;
int buzzerPin = 8;
int ledPin = 13;
int servoPin = 9;

Servo myServo;

// Function: Baca nilai sensor
int bacaSensor() {
  return analogRead(mq2Pin);
}

// Function: Cek apakah asap melebihi batas (return boolean)
boolean cekAsap(int nilai) {
  return (nilai > batasAsap);
}


// Function: Aktifkan alarm (void)
void aktifkanAlarm(boolean asapTerdeteksi) {
  if (asapTerdeteksi) {
    Serial.println("Asap terdeteksi!");
    digitalWrite(buzzerPin, HIGH);  // Buzzer nyala
    digitalWrite(ledPin, HIGH);     // LED nyala
    myServo.write(90);              // Gerak servo (untuk buka jendela)
  } else {
    Serial.println("Udara aman");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    myServo.write(0);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);
}

void loop() {
  int nilaiAsap = bacaSensor();  // Panggil function
  Serial.print("Nilai asap: ");
  Serial.println(nilaiAsap);
  
  boolean asap = cekAsap(nilaiAsap);  // Panggil function
  aktifkanAlarm(asap);                // Panggil function
  
  delay(10000);
}
