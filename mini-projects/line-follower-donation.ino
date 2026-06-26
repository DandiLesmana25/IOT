/*
 * PROJECT:  Line Follower Robot kotak infak otomatis
 * HARDWARE: Arduino Uno + L298N + 2x Gearbox Motor + 2x IR Sensor FC-51 + sensor jarak hc-sr04
 * LOGIC: Garis Hitam (HIGH), Lantai Putih (LOW)
 * LOGIC menghentikan kotak: jika hasil deteksi sensor jarak < 5 cm
 */

// --- 1. DEFINISI PIN SENSOR IR ---
#define SENSOR_KIRI 2
#define SENSOR_KANAN 10

// --- 2. DEFINISI PIN MOTOR KIRI (A) ---
#define ENA 9
#define IN1 8
#define IN2 7
 
// --- 3. DEFINISI PIN MOTOR KANAN (B) ---
#define ENB 3
#define IN3 4
#define IN4 5

// --- 4. SETTING KECEPATAN (0 - 255) ---
// Tuning kecepatan sangat krusial di sini
int speedMaju = 110;  // Jangan terlalu cepat agar sensor tidak terlewat membaca garis
int speedBelok = 120; // Tenaga lebih besar untuk pivot turn (berputar di tempat)

void setup() {
  // Setup Sensor sebagai penerima data (INPUT)
  pinMode(SENSOR_KIRI, INPUT);
  pinMode(SENSOR_KANAN, INPUT);

  // Setup Motor sebagai eksekutor (OUTPUT)
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  
  // Safety delay: Beri waktu 2 detik sebelum robot tiba-tiba meluncur
  delay(2000); 
}

void loop() {
  // --- A. BACA LINGKUNGAN SEKITAR ---
  int kiri = digitalRead(SENSOR_KIRI);
  int kanan = digitalRead(SENSOR_KANAN);

  Serial.println(kiri);
  Serial.println(kanan);

  // --- B. PENGAMBILAN KEPUTUSAN (THE BRAIN) ---
  
  // 1. LURUS: Kedua sensor di atas lantai putih (LOW)
  if (kiri == LOW && kanan == LOW) {
    maju();
  }
  
  // 2. KOREKSI KIRI: Sensor kiri mengenai garis hitam (HIGH)
  // Artinya badan robot sedang melenceng ke kanan, jadi harus banting setir ke kiri
  else if (kiri == HIGH && kanan == LOW) {
    belokKiri();
  }
  
  // 3. KOREKSI KANAN: Sensor kanan mengenai garis hitam (HIGH)
  // Artinya badan robot sedang melenceng ke kiri, jadi harus banting setir ke kanan
  else if (kiri == LOW && kanan == HIGH) {
    belokKanan();
  }
  
  // 4. BERHENTI: Kedua sensor mendeteksi hitam
  // Bertemu garis finish, perempatan, atau diangkat dari lantai
  else if (kiri == HIGH && kanan == HIGH) {
    berhenti();
  }
}

// ==========================================
// FUNGSI PENGGERAK (THE MUSCLES)
// ==========================================

void maju() {
  Serial.println("majuuuu");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedMaju);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedMaju);
}

void belokKiri() {
  Serial.println("kirrrriiiii");
  // Pivot Turn Kiri: Roda kiri mundur, Roda kanan maju
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedBelok);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedBelok);
}

void belokKanan() {
  Serial.println("kanannnn");
  // Pivot Turn Kanan: Roda kiri maju, Roda kanan mundur
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedBelok);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speedBelok);
}

void berhenti() {
  Serial.println("berhentiii");
  // Matikan semua daya ke motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
