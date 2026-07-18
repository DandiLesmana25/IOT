#include <ArduinoBLE.h>
#include "DHT.h" // Memanggil library sensor DHT

// 1. KONFIGURASI SENSOR DHT11
#define DHTPIN 2       // Pin data sensor DHT11 dihubungkan ke Digital Pin 2
#define DHTTYPE DHT11  // Menentukan jenis sensor (DHT11)

// Membuat objek sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// 2. MEMBUAT BUKU MENU BLE
BLEService sensorService("9e154c5c-d036-4089-8ec1-c042eeda5b42"); 

// Item Menu: Sensor Suhu (Tipe Int / Bilangan Bulat)
BLEIntCharacteristic suhuCharacteristic("9e154c5c-d037-4089-8ec1-c042eeda5b42", BLERead | BLENotify);

// Variabel untuk timer telemetri (pengiriman data berkala)
unsigned long waktuSebelumnya = 0;

void setup() {
  Serial.begin(115200);

  // Mulai komunikasi dengan sensor DHT
  dht.begin(); 

  // Mulai komunikasi BLE
  if (!BLE.begin()) {
    Serial.println("Gagal memulai BLE!");
    while (1);
  }

  // 3. MENGATUR IDENTITAS BLUETOOTH
  BLE.setLocalName("Monitor Suhu"); 
  BLE.setAdvertisedService(sensorService);
  
  sensorService.addCharacteristic(suhuCharacteristic);
  BLE.addService(sensorService);

  // Set nilai awal di HP menjadi 0
  suhuCharacteristic.writeValue(0);

  BLE.advertise();
  Serial.println("BLE Aktif. Menunggu koneksi dari HP...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("HP Terkoneksi: ");
    Serial.println(central.address());

    while (central.connected()) {
      
      // LOGIKA TELEMETRI (Mengirim setiap 2 detik / 2000 ms)
      if (millis() - waktuSebelumnya >= 2000) {
        waktuSebelumnya = millis(); // Reset timer
        
        // 4. MEMBACA DATA SENSOR ASLI
        // Sensor DHT11 biasanya membaca suhu dalam tipe desimal (float)
        float suhuAsli = dht.readTemperature();
        
        // FITUR PENGAMANAN (Safety Check)
        // Mengecek apakah pembacaan gagal (Kabel kendor/putus)
        // isnan = Is Not A Number
        if (isnan(suhuAsli)) {
          Serial.println("AWAS: Gagal membaca data dari sensor DHT11! Cek kabel.");
        } else {
          // Jika sukses, ubah angka desimal menjadi angka bulat (Integer) 
          // agar cocok dengan tipe data suhuCharacteristic di atas.
          int suhuBulat = (int)suhuAsli;

          Serial.print("Suhu Ruangan Terdeteksi: ");
          Serial.print(suhuBulat);
          Serial.println(" C");
          
          // LEMPAR DATA KE HP!
          suhuCharacteristic.writeValue(suhuBulat);
        }
      }
      
    } 

    Serial.println("HP Terputus.");
  }
}
