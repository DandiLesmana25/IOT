#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoBLE.h>

// Pin MFRC522 untuk Arduino UNO R4 WiFi
#define RST_PIN         9
#define SS_PIN          10

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Membuat BLE Service & Characteristic baru (UUID berbeda dari project suhu)
BLEService rfidService("20b10000-e8f2-537e-4f6c-d104768a1214");
// Characteristic String (maksimal 20 karakter) karena UID berupa teks misal "A1 B2 C3 D4"
BLEStringCharacteristic rfidCharacteristic("20b10002-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify, 20);

unsigned long lastReadTime = 0;

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi SPI dan RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  if (!BLE.begin()) {
    Serial.println("Gagal memulai BLE!");
    while (1);
  }

  // Setup BLE
  BLE.setLocalName("RFID Scanner");
  BLE.setAdvertisedService(rfidService);
  rfidService.addCharacteristic(rfidCharacteristic);
  BLE.addService(rfidService);
  
  rfidCharacteristic.writeValue("READY"); // Nilai awal
  
  BLE.advertise();
  Serial.println("BLE Aktif. Menunggu koneksi Web...");
  Serial.println("Dekatkan kartu RFID ke modul RC522...");
}

void loop() {
  BLEDevice central = BLE.central();

  // Jika Web Terkoneksi
  if (central) {
    while (central.connected()) {
      
      // 1. Cek apakah ada kartu RFID baru di dekat scanner
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        
        // 2. Proteksi Anti-spam: Jangan baca berulang kali dalam 1.5 detik
        if (millis() - lastReadTime > 1500) {
            
            // 3. Gabungkan Byte menjadi String Hex
            String uidString = "";
            for (byte i = 0; i < mfrc522.uid.size; i++) {
              uidString += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
              uidString += String(mfrc522.uid.uidByte[i], HEX);
              if (i != mfrc522.uid.size - 1) {
                  uidString += " "; // Tambah spasi pemisah
              }
            }
            uidString.toUpperCase(); // Jadikan huruf besar
            
            Serial.print("Kartu Ditemukan! UID: ");
            Serial.println(uidString);
            
            // 4. Kirim UID ke Web Browser
            rfidCharacteristic.writeValue(uidString);
            
            lastReadTime = millis();
        }
        
        // 5. Matikan komunikasi ke kartu agar tidak mengganggu bacaan selanjutnya
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
      }
    }
  }
}
