#include <ArduinoBLE.h>
// 1. MEMBUAT BUKU MENU (UUID)
// UUID ini ibarat "Barcode" unik agar HP tahu ini menu apa. 
// (Bisa diganti angkanya asalkan formatnya sama)
BLEService ledService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Kategori Menu

// Membuat Item Menu: Saklar Lampu (Tipe Data Byte)
// Diberi hak akses BLERead (Bisa dilihat) dan BLEWrite (Bisa diubah/ditekan)
BLEByteCharacteristic saklarCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Pastikan mati di awal

  while (!Serial); // Tunggu Serial Monitor terbuka

  // 1. Nyalakan Modul BLE di dalam R4
  if (!BLE.begin()) {
    Serial.println("Gagal menyalakan modul BLE!");
    while (1); // Berhenti di sini jika error
  }

  // 2. Pasang Nama BLuetoth kita (Local Name)
  BLE.setLocalName("R4_Dandi"); 
  BLE.setAdvertisedService(ledService);
  // Memasukkan Item Menu (Saklar) ke dalam Kategori Menu (Service)
  ledService.addCharacteristic(saklarCharacteristic);
  // Memasukkan seluruh Kategori Menu ke dalam sistem BLE Arduino
  BLE.addService(ledService);
  // Set nilai awal saklar menjadi 0 (Mati)
  saklarCharacteristic.writeValue(0);

  // 3. Mulai menyebar sinyak agar terlihat oleh HP (Advertising)
  BLE.advertise();

  Serial.println("Bluetooth aktif! Silakan buka aplikasi nRF Connect/LightBlue di HP kalian.");
}

void loop() {
  // Biarkan R4 menunggu ada HP yang masuk (Central)
  BLEDevice central = BLE.central();

  // Jika ada pelanggan (HP) yang terkoneksi
  if (central) {
    Serial.print("Ada HP yang terkoneksi! MAC Address: ");
    Serial.println(central.address());

   

    // Selama HP masih terkoneksi, program akan berputar di dalam fungsi while ini
    while (central.connected()) {
      // 3. MENERIMA PESANAN (Mengecek apakah ada data baru dari HP)
      if (saklarCharacteristic.written()) {
        if (saklarCharacteristic.value() == 1) { // Jika HP mengirim angka 1
          Serial.println("Perintah diterima: Nyalakan Lampu!");
          digitalWrite(LED_BUILTIN, HIGH);
          
        } else if (saklarCharacteristic.value() == 0) { // Jika HP mengirim angka 0
          Serial.println("Perintah diterima: Matikan Lampu!");
          digitalWrite(LED_BUILTIN, LOW);
        }
        
      }
    }

    // Jika HP terputus (disconnect)
    Serial.println("HP terputus.");
    digitalWrite(LED_BUILTIN, LOW); // Matikan LED
  }
}
