#include <ArduinoBLE.h>

// 1. BUAT KOTAK SURAT (Service & Characteristic)
// Kita buat Kategori Menu (Service) dengan UUID kustom sederhana
BLEService customService("FFF0"); 

// Kita buat Wadah Data (Characteristic) yang bisa dibaca (Read) dan ditulis (Write) oleh HP
// Tipe datanya Byte (bisa menerima angka atau karakter huruf)
BLEByteCharacteristic rxCharacteristic("FFF1", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);
  while (!Serial); // Tunggu Serial Monitor terbuka

  // Set LED bawaan sebagai output
  pinMode(LED_BUILTIN, OUTPUT);

  // Nyalakan Modul BLE di dalam R4
  if (!BLE.begin()) {
    Serial.println("Gagal menyalakan modul BLE!");
    while (1); 
  }

  // Pasang Nama Bluetooth kita (Local Name)
  BLE.setLocalName("R4_Dandi"); 
  
  // 2. DAFTARKAN STRUKTUR BLE KE SISTEM
  BLE.setAdvertisedService(customService); // Pasang Service di brosur pancaran sinyal
  customService.addCharacteristic(rxCharacteristic); // Masukkan wadah data ke dalam Service
  BLE.addService(customService); // Aktifkan Service di board R4

  // Beri nilai awal pada wadah data
  rxCharacteristic.writeValue(0);

  // Mulai menyebar sinyal agar terlihat oleh HP (Advertising)
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
      
      // LOGIKA BARU: Periksa apakah HP telah menulis/mengirimkan data ke wadah 'rxCharacteristic'
      if (rxCharacteristic.written()) {
        
        // Ambil data byte yang dikirim oleh HP
        byte dataMasuk = rxCharacteristic.value(); 
        
        Serial.print("Data mentah diterima (Hex/Byte): ");
        Serial.println(dataMasuk);

        // KONDISI 1: Jika HP mengirim angka 1 (atau karakter yang sesuai)
        if (dataMasuk == 1 || dataMasuk == '1') {
          Serial.println("-> LOGIK: HP mengirim perintah NYALA.");
          Serial.println("-> STATUS: Menampilkan teks sukses di Serial Monitor!\n");
          
          // Anda juga bisa menambahkan aksi fisik di sini, misal:
          // digitalWrite(pinLED, HIGH); 
        } 
        // KONDISI 2: Jika HP mengirim angka 0
        else if (dataMasuk == 0 || dataMasuk == '0') {
          Serial.println("-> LOGIK: HP mengirim perintah MATI.");
          Serial.println("-> STATUS: Menampilkan teks matikan sistem!\n");
          
          // digitalWrite(pinLED, LOW);
        } 
        // KONDISI LAIN: Jika mengirim karakter teks bebas (misal huruf A, B, C)
        else {
          Serial.print("-> LOGIK: HP mengirim karakter teks: ");
          Serial.println((char)dataMasuk); // Mengubah byte menjadi karakter huruf asli
          Serial.println();
        }
      }
      
    } // Akhir dari hubungan connected

    // Jika HP terputus (disconnect)
    Serial.println("HP terputus.");
    digitalWrite(LED_BUILTIN, LOW); // Matikan LED penanda koneksi
  }
}
