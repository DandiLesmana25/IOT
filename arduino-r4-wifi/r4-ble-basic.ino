#include <ArduinoBLE.h>

void setup() {
  Serial.begin(115200);
  while (!Serial); // Tunggu Serial Monitor terbuka

  // 1. Nyalakan Modul BLE di dalam R4
  if (!BLE.begin()) {
    Serial.println("Gagal menyalakan modul BLE!");
    while (1); // Berhenti di sini jika error
  }

  // 2. Pasang Nama BLuetoth kita (Local Name)
  BLE.setLocalName("R4_Dandi"); 

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

    // Nyalakan LED bawaan sebagai tanda HP sudah tersambung
    digitalWrite(LED_BUILTIN, HIGH);

    // Selama HP masih terkoneksi, program akan berputar di dalam fungsi while ini
    while (central.connected()) {
    //  buatkan logic menampilkan text  jika mendapat value dari HP
      // Contoh: jika HP mengirimkan data tertentu, tampilkan pesan di Serial Monitor
      if (central.available()) {
      
      }
    }

    // Jika HP terputus (disconnect)
    Serial.println("HP terputus.");
    digitalWrite(LED_BUILTIN, LOW); // Matikan LED
  }
}