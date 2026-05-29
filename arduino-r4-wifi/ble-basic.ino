#include <ArduinoBLE.h>

// 1. Definisikan UUID untuk Service dan Characteristic
// Anda bisa men-generate UUID acak di internet, ini contoh standar
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 

// Characteristic bertipe Byte (1 byte data) dengan akses BACA dan TULIS
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // Menggunakan LED kuning bawaan board

void setup() {
  Serial.begin(115200);
  while (!Serial); // Hapus baris ini jika ingin jalan tanpa colok USB ke PC
  
  pinMode(ledPin, OUTPUT);

  // Inisialisasi modul BLE
  if (!BLE.begin()) {
    Serial.println("Gagal menginisialisasi modul BLE!");
    while (1); 
  }

  // 2. Setel nama perangkat yang akan muncul di HP Anda
  BLE.setLocalName("RoboBLE_Dandi");
  
  // 3. Daftarkan Service dan Characteristic
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);

  // Setel nilai awal Characteristic menjadi 0 (Mati)
  switchCharacteristic.writeValue(0);

  // 4. Mulai memancarkan sinyal Bluetooth
  BLE.advertise();
  Serial.println("BLE Aktif! Menunggu koneksi dari HP...");
}

void loop() {
  // Tunggu hingga ada perangkat Central (HP) yang terkoneksi
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Terhubung ke MAC Address: ");
    Serial.println(central.address());

    // Selama HP masih terkoneksi, jalankan logika ini
    while (central.connected()) {
      
      // Jika ada data baru yang ditulis dari HP
      if (switchCharacteristic.written()) {
        
        // Cek nilai datanya (1 = ON, 0 = OFF)
        if (switchCharacteristic.value() == 1) {   
          Serial.println("Perintah diterima: LED ON");
          digitalWrite(ledPin, HIGH);
        } else {                              
          Serial.println("Perintah diterima: LED OFF");
          digitalWrite(ledPin, LOW);
        }
      }
    }

    // Jika keluar dari loop while, berarti HP terputus
    Serial.print("Terputus dari: ");
    Serial.println(central.address());
  }
}