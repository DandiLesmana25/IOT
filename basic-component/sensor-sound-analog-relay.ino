/*
 * PROJECT: Clap Switch (Saklar Tepuk)
 * HARDWARE: Arduino Uno + Modul Sensor Suara (KY-038)
 */
#define PIN_SENSOR A0
#define PIN_LAMPU 7 // Bisa diganti ke pin Relay

// Variabel untuk menyimpan status lampu saat ini (menyala/mati)
bool statusLampu = false; 



// --- KALIBRASI SENSITIVITAS ---
// Rentang nilai analog adalah 0 - 1023.
// Saat diam (hening), nilai analog suara biasanya stabil di angka tertentu (misal: 400 atau 500).
// Saat ditepuk, nilainya akan melonjak tajam atau drop tajam.
// Angka ini WAJIB disesuaikan dengan hasil pantauan di Serial Monitor.
int ambangBatas = 600; // Contoh: Jika nilai melompat di atas 600, anggap itu tepukan.

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_LAMPU, OUTPUT);
  
  // Pastikan lampu mati di awal
  digitalWrite(PIN_LAMPU, HIGH);
  
  Serial.println("Sistem Saklar Tepuk Siap!");
}

void loop() {
  // 1. Baca nilai amplitudo suara secara real-time
  int nilaiSuara = analogRead(PIN_SENSOR);
  
  // (OPSIONAL: Buka komentar ini saat pertama kali kalibrasi untuk melihat angka hening ruangan)
  // Serial.println(nilaiSuara); 
  
  // 2. Logika Deteksi Tepukan (Software Threshold)
  // Jika gelombang suara lebih besar dari ambang batas yang kita tentukan
  if (nilaiSuara > ambangBatas) {
    
    Serial.print("Tepukan Terdeteksi! Kekuatan: ");
    Serial.println(nilaiSuara);
    
    // --- KONSEP TOGGLE ---
    statusLampu = !statusLampu; 
    
    // Eksekusi ke lampu/relay
    if (statusLampu == true) {
      digitalWrite(PIN_LAMPU, LOW);
      Serial.println("-> Lampu NYALA");
    } else {
      digitalWrite(PIN_LAMPU, HIGH);
      Serial.println("-> Lampu MATI");
    }
    
    // --- SAFETY DELAY (DEBOUNCING) ---
    // delay ke 1000ms (1 detik). 
    delay(1000); 
  }
}
