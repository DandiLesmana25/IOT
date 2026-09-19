/*
 * PROJECT: Clap Switch (Saklar Tepuk)
 * HARDWARE: Arduino Uno + Modul Sensor Suara (KY-038)
 */

#define PIN_SENSOR 8
#define PIN_LAMPU 7 // Bisa diganti ke pin Relay

// Variabel untuk menyimpan status lampu saat ini (menyala/mati)
bool statusLampu = false; 

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_LAMPU, OUTPUT);
  
  // Pastikan lampu mati di awal
  digitalWrite(PIN_LAMPU, LOW);
  
  Serial.println("Sistem Saklar Tepuk Siap!");
}

void loop() {
  // 1. Baca data dari sensor suara
  // Biasanya modul ini mengirim HIGH saat ada suara keras (tepukan)
  int suaraMasuk = digitalRead(PIN_SENSOR);
  
  // 2. Logika Deteksi Tepukan
  if (suaraMasuk == HIGH) {
    Serial.println("Tepukan Terdeteksi!");
    
    // --- KONSEP TOGGLE ---
    // Membalikkan status: Jika False jadi True, jika True jadi False
    statusLampu = !statusLampu; 
    
    // Eksekusi status terbaru ke lampu/relay
    if (statusLampu == true) {
      digitalWrite(PIN_LAMPU, HIGH);
      Serial.println("-> Lampu NYALA");
    } else {
      digitalWrite(PIN_LAMPU, LOW);
      Serial.println("-> Lampu MATI");
    }
    
    // --- SAFETY DELAY (DEBOUNCING)  agar tidak mendeteksi terlalu cepat---
    delay(3000); 
  }
}
