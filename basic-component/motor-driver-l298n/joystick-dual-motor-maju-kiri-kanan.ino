/*
 * PROJECT: Analog Joystick Motor Control
 * HARDWARE: Arduino Uno + L298N + Joystick Module + 1x DC Motor
 * LOGIC: Analog Read, Deadzone, dan Konversi Skala (Mapping)
 */

// --- 1. DEFINISI PIN ---
#define PIN_Y A0   // Sumbu Y Joystick (Maju/Mundur)
#define PIN_X A1   // Sumbu X Joystick (Belok Kiri/Kanan) 


// roda 1
// Pin Motor A (L298N)
#define ENA 9         // Kontrol Kecepatan (Wajib Pin PWM)
#define IN1 8      // Kontrol Arah 1
#define IN2 7      // Kontrol Arah 2


// roda 2
#define ENB 3        // Kontrol Kecepatan (Wajib Pin PWM)
#define IN3 6      // Kontrol Arah 1
#define IN4 5      // Kontrol Arah 2

// --- 2. KONSTANTA DEADZONE ---
#define DEADZONE_Y_LOW 400
#define DEADZONE_Y_HIGH 600
#define DEADZONE_X_LOW 400
#define DEADZONE_X_HIGH 600

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.println("=== ROBOT MOTION CONTROL STARTED ===");
}







void loop() {
  // ===== 1. BACA INPUT JOYSTICK =====
  int nilaiY = analogRead(PIN_Y);  // Maju/Mundur (Forward/Backward)
  int nilaiX = analogRead(PIN_X);  // Belok Kiri/Kanan (Left/Right Turn)

  // ===== 2. HITUNG KECEPATAN BASE (dari Y-axis) =====
  int baseSpeed = 0;
  int motorDirection = 0;  // 1=Maju, -1=Mundur, 0=Stop

  if (nilaiY < DEADZONE_Y_LOW) {
    // MAJU
    baseSpeed = map(nilaiY, DEADZONE_Y_LOW, 0, 0, 255);
    motorDirection = 1;  // Forward
  }
  else if (nilaiY > DEADZONE_Y_HIGH) {
    // MUNDUR
    baseSpeed = map(nilaiY, DEADZONE_Y_HIGH, 1023, 0, 255);
    motorDirection = -1;  // Backward
  }
  else {
    // DEADZONE Y - Stop
    baseSpeed = 0;
    motorDirection = 0;
  }

  // ===== 3. HITUNG TURN FACTOR (dari X-axis) =====
  int turnFactor = 0;  // -255 sampai 255 (negatif=kiri, positif=kanan)

  if (nilaiX < DEADZONE_X_LOW) {
    // BELOK KIRI
    turnFactor = map(nilaiX, DEADZONE_X_LOW, 0, 0, -255);
  }
  else if (nilaiX > DEADZONE_X_HIGH) {
    // BELOK KANAN
    turnFactor = map(nilaiX, DEADZONE_X_HIGH, 1023, 0, 255);
  }
  else {
    // DEADZONE X - Jalan lurus
    turnFactor = 0;
  }

  // ===== 4. HITUNG KECEPATAN MASING-MASING MOTOR =====
  int speedMotorA = baseSpeed;  // Motor Kiri
  int speedMotorB = baseSpeed;  // Motor Kanan

  // Aplikasikan turn factor
  speedMotorA = constrain(baseSpeed + turnFactor, 0, 255);
  speedMotorB = constrain(baseSpeed - turnFactor, 0, 255);

  // ===== 5. KONTROL MOTOR =====
  if (motorDirection == 1) {
    // MAJU
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedMotorA);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speedMotorB);
  }
  else if (motorDirection == -1) {
    // MUNDUR
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speedMotorA);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, speedMotorB);
  }
  else {
    // STOP (Deadzone Y)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }

  // ===== 6. OUTPUT DEBUG SERIAL =====
  Serial.print("Y:");
  Serial.print(nilaiY);
  Serial.print(" | X:");
  Serial.print(nilaiX);
  Serial.print(" | DIR:");
  if (motorDirection == 1) Serial.print("MAJU");
  else if (motorDirection == -1) Serial.print("MUNDUR");
  else Serial.print("STOP");
  Serial.print(" | SpeedA:");
  Serial.print(speedMotorA);
  Serial.print(" | SpeedB:");
  Serial.print(speedMotorB);
  Serial.print(" | Turn:");
  Serial.print(turnFactor);
  Serial.println();

  delay(50);
}