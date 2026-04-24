// --- 1. DEFINISI PIN JOYSTICK ---
#define PIN_Y A0 // Sumbu Y (Maju/Mundur)

// --- 2. DEFINISI PIN MOTOR KIRI (A) ---
#define ENA 9    // Kontrol Kecepatan (Wajib Pin PWM)
#define IN1 8    // Kontrol Arah 1
#define IN2 7    // Kontrol Arah 2

// --- 3. DEFINISI PIN MOTOR KANAN (B) ---
#define ENB 3    // Kontrol Kecepatan (Wajib Pin PWM)
#define IN3 5    // Kontrol Arah 1
#define IN4 4    // Kontrol Arah 2

void setup() {
  Serial.begin(9600);
  
  // Set semua pin motor sebagai OUTPUT
  pinMode(ENA, OUTPUT);   
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
}


void loop() {
  int nilaiY = analogRead(PIN_Y);
  int kecepatan = 0;

  // --- LOGIKA KONTROL (BRAIN) ---

  // 1. Logika untuk MAJU
  if (nilaiY < 400) {
    kecepatan = map(nilaiY, 400, 0, 0, 255);
    maju(kecepatan);
  } 
  
  // 2. Logika untuk MUNDUR
  else if (nilaiY > 600) {
    kecepatan = map(nilaiY, 600, 1023, 0, 255);
    mundur(kecepatan);
  } 
  
  // 3. Logika untuk BERHENTI (Deadzone)
  else {
    berhenti();
  }

  // Delay kecil untuk kestabilan serial monitor
  delay(50);
}


// ==========================================
// FUNGSI PENGGERAK MOTOR (MUSCLES)
// ==========================================

void maju(int speed) {
  // Motor Kiri
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  // Motor Kanan
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);

  Serial.println("Majuuu");
  Serial.print("Speed: ");
  Serial.println(speed);
}


void mundur(int speed) {
  // Motor Kiri
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  // Motor Kanan
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);

  Serial.println("Mundur broo");
  Serial.print("Kecepatan: ");
  Serial.println(speed);
}


void berhenti() {
  // Motor Kiri
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  // Motor Kanan
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);

 Serial.print("berhentii") 
}
