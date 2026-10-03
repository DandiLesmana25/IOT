/*
 * PROJECT: RFID Smart Door Lock (Multi-Card Support)
 * HARDWARE: Arduino, RC522, Relay, Solenoid 12V, Buzzer
 */

#include <SPI.h>
#include <MFRC522.h>

// Definisi Pin
#define RST_PIN   9
#define SS_PIN    10
#define PIN_RELAY 2
#define PIN_BUZZER 7

// Buat objek RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ======================================================
// DAFTAR KARTU TERDAFTAR (ARRAY 2 DIMENSI)
// Tambahkan UID kartu/tag baru di dalam kurung kurawal ini
// ======================================================
const byte daftarKartu[][4] = {
  {0x93, 0x4F, 0xE7, 0x00}, // Kartu 1 
  {0xDE, 0xAD, 0xBE, 0xEF}, // Kartu 2 (
  {0xA1, 0xB2, 0xC3, 0xD4}, // Kartu 3 (
  {0x12, 0x34, 0x56, 0x78}  // Kartu 4 
};

// Hitung jumlah kartu terdaftar secara otomatis
const byte JUMLAH_KARTU = sizeof(daftarKartu) / sizeof(daftarKartu[0]);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();        // Inisialisasi bus SPI
  mfrc522.PCD_Init(); // Inisialisasi modul RFID RC522

  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Setelan awal: Pintu terkunci (Relay OFF)
  digitalWrite(PIN_RELAY, HIGH); 
  
  Serial.println("Sistem Smart Door Lock Aktif!");
  Serial.print("Total Kartu Terdaftar: ");
  Serial.println(JUMLAH_KARTU);
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

  // 3. Tampilkan UID Kartu ke Serial Monitor
  Serial.print("UID Kartu Terdeteksi: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // 4. Verifikasi UID ke semua kartu terdaftar
  if (verifikasiAkses(mfrc522.uid.uidByte, mfrc522.uid.size)) {
    Serial.println("Akses Diterima! Pintu Terbuka.");
    bukaPintu();
  } else {
    Serial.println("Akses Ditolak! Kartu Tidak Dikenal.");
    tolakAkses();
  }

  // Hentikan pembacaan kartu saat ini (debouncing)
  mfrc522.PICC_HaltA();
}

// ==========================================
// FUNGSI VERIFIKASI BANYAK KARTU
// ==========================================
bool verifikasiAkses(byte *uidDibaca, byte ukuranUid) {
  // Pastikan panjang UID adalah 4 byte
  if (ukuranUid != 4) return false;

  // Periksa ke setiap kartu di dalam daftar
  for (byte k = 0; k < JUMLAH_KARTU; k++) {
    bool cocok = true;
    
    // Cocokkan byte per byte (4 byte)
    for (byte b = 0; b < 4; b++) {
      if (uidDibaca[b] != daftarKartu[k][b]) {
        cocok = false;
        break; // Jika ada 1 byte beda, lewati ke kartu berikutnya
      }
    }

    // Jika ke-4 byte cocok sepenuhnya dengan salah satu kartu
    if (cocok) {
      return true; // Akses Diterima!
    }
  }

  return false; // Tidak ada kartu yang cocok
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
  digitalWrite(PIN_RELAY, HIGH);
  Serial.println("Pintu Kembali Terkunci.");
}

void tolakAkses() {
  // Bunyi beep gagal (Nada rendah panjang)
  tone(PIN_BUZZER, 500, 1000); 
  
  // Pintu dipastikan tetap terkunci
  digitalWrite(PIN_RELAY, HIGH); 
}
