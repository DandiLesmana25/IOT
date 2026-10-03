/*
 * PROJECT: RFID Smart Door Lock
 * HARDWARE: Arduino, RC522, Relay, Solenoid 12V, Buzzer
 */

#include <SPI.h>
#include <MFRC522.h>

// Definisi Pin
#define RST_PIN   9
#define SS_PIN    10
// #define PIN_RELAY 8
#define PIN_RELAY 2
#define PIN_BUZZER 7

// Buat objek RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// --- GANTI DENGAN UID KARTU ANDA ---
// Anda akan melihat format ini di Serial Monitor saat kartu ditempelkan
// byte kartuTerdaftar[] = {0xDE, 0xAD, 0xBE, 0xEF}; 
byte kartuTerdaftar[] = {0x93, 0x4F, 0xE7, 0x00};

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();        // Inisialisasi bus SPI
  mfrc522.PCD_Init(); // Inisialisasi modul RFID RC522

  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Setelan awal: Pintu terkunci (Relay OFF)
  // Ubah menjadi HIGH atau LOW tergantung jenis modul relay Anda (Active High/Low)
  digitalWrite(PIN_RELAY, HIGH); 
  
  Serial.println("Sistem Smart Door Lock Aktif!");
  Serial.println("Silakan tempelkan kartu Anda...");
}

void loop() {
  // 1. Cek apakah ada kartu baru yang ditempelkan
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // 2. Baca nomor seri (UID) kartu tersebut
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // 3. Tampilkan UID Kartu ke Serial Monitor (Untuk mendaftarkan kartu baru)
  Serial.print("UID Kartu Terdeteksi: ");
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // 4. Proses Verifikasi
  bool aksesDiberikan = true;
  
  // Cocokkan ukuran UID
  if (mfrc522.uid.size == 4) { 
    // Cocokkan setiap byte dari UID
    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != kartuTerdaftar[i]) {
        aksesDiberikan = false;
        break;
      }
    }
  } else {
    aksesDiberikan = false; // Tolak jika format kartu berbeda
  }

  // 5. Eksekusi Berdasarkan Hasil Verifikasi
  if (aksesDiberikan) {
    Serial.println("Akses Diterima! Pintu Terbuka.");
    bukaPintu();
  } else {
    Serial.println("Akses Ditolak! Kartu Tidak Dikenal.");
    tolakAkses();
  }

  // Hentikan pembacaan kartu saat ini agar tidak berulang-ulang terbaca (debouncing)
  mfrc522.PICC_HaltA();
}

// ==========================================
// FUNGSI AKSI
// ==========================================
void bukaPintu() {
  // Bunyi beep sukses (2 nada cepat)
  tone(PIN_BUZZER, 2000, 100);
  delay(150);
  tone(PIN_BUZZER, 2500, 200);

  // Buka pengunci pintu (Relay ON)
  digitalWrite(PIN_RELAY, LOW); 
  
  // Tahan pintu terbuka selama 5 detik
  delay(5000); 
  
  // Kunci kembali pintu (Relay OFF)
  digitalWrite(PIN_RELAY, HIG  H);
  Serial.println("Pintu Kembali Terkunci.");
}

void tolakAkses() {
  // Bunyi beep gagal (Nada rendah panjang)
  tone(PIN_BUZZER, 500, 1000); 
  
  // Pintu dipastikan tetap terkunci
  digitalWrite(PIN_RELAY, HIGH); 
}

