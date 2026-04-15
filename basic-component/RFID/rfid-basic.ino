/*
 * Basic Syntax: Membaca UID Kartu RFID RC522
 */

#include <SPI.h>
#include <MFRC522.h>

// 1. DEFINISI PIN
#define SS_PIN 10  // Pin SDA
#define RST_PIN 9  // Pin Reset

// 2. INSTANSIASI OBJEK
// Membuat object bernama 'rfid' dari class MFRC522
MFRC522 rfid(SS_PIN, RST_PIN); 

void setup() {
  // Inisialisasi komunikasi serial ke PC
  Serial.begin(9600);
  
  // Inisialisasi bus SPI
  SPI.begin();      
  
  // Inisialisasi modul RFID (PCD - Proximity Coupling Device)
  rfid.PCD_Init(); 

  Serial.println("Dekatkan kartu RFID ke pembaca...");
  Serial.println();
}



void loop() {
  // 3. LOGIKA DETEKSI KARTU
  
  // Cek apakah ada kartu baru terdeteksi? 
  // Jika TIDAK ada (!), maka kembali ke awal loop (return)
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Cek apakah kartu bisa dibaca serialnya?
  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }

  // 4. MENAMPILKAN UID
  Serial.print("UID Tag :");
  
  // Loop sebanyak ukuran byte UID (biasanya 4 byte untuk kartu standar)
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "); // Tambah 0 jika angka < 10 (hex formatting)
    Serial.print(rfid.uid.uidByte[i], HEX); // Cetak dalam format Hexadesimal
  }
  
  Serial.println(); // Baris baru

  // 5. MENGHENTIKAN KOMUNIKASI KARTU
  // Penting agar modul siap membaca kartu berikutnya
  rfid.PICC_HaltA();
}

