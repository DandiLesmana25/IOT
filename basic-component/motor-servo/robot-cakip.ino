#include <Servo.h>

// Membuat objek servo
Servo myServo;

// Pin servo
int servoPin = 9;

void setup() {
  // Menghubungkan servo ke pin 9
  myServo.attach(servoPin);
}

void loop() {

  // Servo ke 0 derajat
  myServo.write(0);
  delay(1000);


  // Servo ke 180 derajat
  myServo.write(180);
  delay(1000);
}