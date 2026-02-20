/*
 * PROJECT: Simple Line Follower Robot
 * HARDWARE: Arduino Uno + L298N + 2x IR Sensor
 */

// --- 1. DEFINISI PIN SENSOR ---
#define SENSOR_KIRI 2
#define SENSOR_KANAN 3

// --- 2. DEFINISI PIN MOTOR (L298N) ---
// Motor Kiri (A)
#define ENA 9  // Enable A (PWM Speed Control)
#define IN1 4  // Input 1
#define IN2 5  // Input 2

// Motor Kanan (B)
#define ENB 10 // Enable B (PWM Speed Control)
#define IN3 6  // Input 3
#define IN4 7  // Input 4

// --- 3. SETTING KECEPATAN (0 - 255) ---
int speedMaju = 120;  // Jangan terlalu ngebut nanti tergelincir
int speedBelok = 150; // Belok butuh tenaga lebih besar

void setup() {
  // Config Sensor sebagai INPUT
  pinMode(SENSOR_KIRI, INPUT);
  pinMode(SENSOR_KANAN, INPUT);
  
  // Config Motor sebagai OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600); // Untuk debugging sensor
}

void loop() {
  // Baca Data Sensor
  // Ingat: LOW = Putih (Pantul), HIGH = Hitam (Serap)
  int bacaKiri = digitalRead(SENSOR_KIRI);
  int bacaKanan = digitalRead(SENSOR_KANAN);

  // --- LOGIKA KONTROL ---
  
  // 1. KONDISI MAJU: Keduanya di atas Putih (Garis ada di tengah robot)
  if (bacaKiri == LOW && bacaKanan == LOW) {
    gerakMaju();
  }
  
  // 2. KONDISI BELOK KIRI: Sensor Kiri kena Hitam
  else if (bacaKiri == HIGH && bacaKanan == LOW) {
    belokKiri();
  }
  
  // 3. KONDISI BELOK KANAN: Sensor Kanan kena Hitam
  else if (bacaKiri == LOW && bacaKanan == HIGH) {
    belokKanan();
  }
  
  // 4. KONDISI STOP: Keduanya Hitam (Garis Finish)
  else if (bacaKiri == HIGH && bacaKanan == HIGH) {
    berhenti();
  }
}



void gerakMaju() {
  // Motor Kiri Maju
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedMaju);

  // Motor Kanan Maju
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedMaju);
}

void belokKiri() {
  // Konsep: Tank Turn (Roda Kiri Mundur/Diam, Roda Kanan Maju)
  
  // Motor Kiri STOP (atau Mundur dikit biar tajam)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW); // Bisa diganti HIGH untuk mundur
  analogWrite(ENA, 0);

  // Motor Kanan MAJU
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedBelok);
}

void belokKanan() {
  // Motor Kiri MAJU
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedBelok);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void berhenti() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  // Motor Kanan STOP
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
