/*
 * PROJECT: Analog Joystick Motor Control
 * HARDWARE: Arduino Uno + L298N + Joystick Module + 1x DC Motor
 * LOGIC: Analog Read, Deadzone, dan Konversi Skala (Mapping)
 */

// --- 1. DEFINISI PIN ---
#define PIN_Y A0   // Sumbu Y Joystick (Maju/Mundur)


// roda 1
// Pin Motor A (L298N)
#define ENA 9         // Kontrol Kecepatan (Wajib Pin PWM)
#define IN1 8      // Kontrol Arah 1
#define IN2 7      // Kontrol Arah 2


// roda 2
#define ENB 3        // Kontrol Kecepatan (Wajib Pin PWM)
#define IN3 6      // Kontrol Arah 1
#define IN4 5      // Kontrol Arah 2

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}







void loop() {
  // 1. Baca nilai sumbu Y dari Joystick (Hasil: 0 - 1023)

  int nilaiY = analogRead(PIN_Y);
  int kecepatan = 0; // Variabel untuk menyimpan hasil konversi PWM

  // --- 2. LOGIKA KONTROL (DEADZONE & MAPPING) ---

  // KONDISI A: Joystick didorong ke ATAS (Nilai < 400)
  // Catatan: Arah dorong bisa menghasilkan 0 atau 1023 tergantung posisi fisik joystick.
  if (nilaiY < 400) {
    // Ubah skala: 400 (pelan) menuju 0 (ngebut) -> dikonversi jadi 0 sampai 255
    kecepatan = map(nilaiY, 400, 0, 0, 255); 
    
    // Motor kiri Maju
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, kecepatan);
    

    // Motor kanan Maju
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, kecepatan);

    Serial.print("MAJU | Speed: ");
    Serial.println(kecepatan);
  }
  
  // KONDISI B: Joystick ditarik ke BAWAH (Nilai > 600)
  else if (nilaiY > 600) {
    // Ubah skala: 600 (pelan) menuju 1023 (ngebut) -> dikonversi jadi 0 sampai 255
    kecepatan = map(nilaiY, 600, 1023, 0, 255);
    
    // Motor Mundur (Logika IN1 dan IN2 dibalik)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, kecepatan);

    // Motor kanan Mundur
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, kecepatan);

    Serial.print("MUNDUR | Speed: ");
    Serial.println(kecepatan);
  }
  
  // KONDISI C: Joystick di TENGAH (Deadzone 400 - 600)
  else {
    // Motor Berhenti
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
    
    Serial.println("STOP (Deadzone)");
  }
  
  // Delay kecil untuk stabilitas pembacaan serial
  delay(50);
}