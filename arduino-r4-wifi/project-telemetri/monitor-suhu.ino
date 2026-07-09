#include <ArduinoBLE.h>

// 1. MEMBUAT BUKU MENU BARU
BLEService sensorService("19b10000-e8f2-537e-4f6c-d104768a1214"); 

// Membuat Item Menu: Sensor Suhu (Tipe Int / Bilangan Bulat)
// Diberi hak akses BLERead (Bisa dibaca manual) dan BLENotify (Bisa mengirim otomatis)
BLEIntCharacteristic suhuCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);

// Variabel untuk timer loper koran (Telemetri)
unsigned long waktuSebelumnya = 0;
int suhuVirtual = 25; // Suhu awal

void setup() {
  Serial.begin(115200);

  if (!BLE.begin()) {
    Serial.println("Gagal memulai BLE!");
    while (1);
  }

  // 2. MENGATUR RESTORAN
  BLE.setLocalName("Monitor Suhu"); 
  BLE.setAdvertisedService(sensorService);
  
  sensorService.addCharacteristic(suhuCharacteristic);
  BLE.addService(sensorService);

  // Nilai awal sensor
  suhuCharacteristic.writeValue(suhuVirtual);

  BLE.advertise();
  Serial.println("BLE Aktif. Menunggu koneksi dari HP...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.println("HP Terkoneksi!");

    while (central.connected()) {
      
      // 3. LOGIKA TELEMETRI (Mengirim setiap 2 detik / 2000 ms)
      if (millis() - waktuSebelumnya >= 2000) {
        waktuSebelumnya = millis(); // Reset timer
        
        // Membuat simulasi data suhu berfluktuasi antara 25 sampai 32 derajat
        // (Bisa diganti dengan analogRead(A0) jika siswa memakai sensor asli)
        suhuVirtual = random(25, 33); 
        
        Serial.print("Update Suhu Ruangan: ");
        Serial.print(suhuVirtual);
        Serial.println(" C");
        
        // LEMPAR DATA KE HP!
        suhuCharacteristic.writeValue(suhuVirtual);
      }
      
    } 

    Serial.println("HP Terputus.");
  }
}
