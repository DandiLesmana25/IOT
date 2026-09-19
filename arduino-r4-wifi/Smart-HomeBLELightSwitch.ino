/*
 * HARDWARE: Arduino Uno R4 WiFi + Modul Relay 1-Channel + Lampu DC
 */

#include <ArduinoBLE.h>
#include <Arduino_LED_Matrix.h>

// --- INISIALISASI LED MATRIX ---
ArduinoLEDMatrix matrix;

// Ikon untuk LED Matrix (Nyala = Kotak Penuh, Mati = Kotak Kosong/Titik)
const uint32_t ikonNyala[] = {
  0xffffffff, 0xffffffff, 0xffffffff
};
const uint32_t ikonMati[] = {
  0x00000000, 0x00000000, 0x00000000
};

// --- INISIALISASI BLE ---
// Membuat custom UUID untuk layanan (Service) dan karakteristik lampu
BLEService lightService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEByteCharacteristic switchChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// --- DEFINISI PIN ---
#define PIN_RELAY 7

void setup() {
  Serial.begin(9600);

  // Setup Relay (Active LOW)
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); // Matikan lampu di awal

  // Setup LED Matrix
  matrix.begin();
  matrix.loadFrame(ikonMati);

  // Setup BLE
  if (!BLE.begin()) {
    Serial.println("Gagal memulai modul BLE!");
    while (1);
  }

  // Konfigurasi Identitas Bluetooth
  BLE.setLocalName("SmartLight_R4"); // Nama yang akan muncul di HP
  BLE.setAdvertisedService(lightService);
  lightService.addCharacteristic(switchChar);
  BLE.addService(lightService);

  // Set nilai awal karakteristik ke 0 (OFF)
  switchChar.writeValue(0); 

  // Mulai memancarkan sinyal Bluetooth
  BLE.advertise();

  Serial.println("Sistem Smart Light Siap!");
  Serial.println("Silakan koneksikan via Bluetooth dari HP.");
}

void loop() {
  // Tunggu ada HP yang terkoneksi
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Terhubung ke HP dengan MAC Address: ");
    Serial.println(central.address());

    // Selama HP terhubung, cek apakah ada perintah yang dikirim
    while (central.connected()) {
      
      // Jika ada data baru yang masuk dari HP
      if (switchChar.written()) {
        char perintah = (char)switchChar.value();
        
        // Panggil fungsi otak pemroses perintah
        eksekusiPerintah(perintah);
      }
    }
    
    // Jika HP terputus (keluar jangkauan)
    Serial.println("Koneksi HP terputus.");
  }
}

// ==========================================
// FUNGSI PENGOLAH PERINTAH (THE BRAIN)
// ==========================================
void eksekusiPerintah(char cmd) {
  if (cmd == '1') {
    nyalakanLampu();
  } 
  else if (cmd == '0') {
    matikanLampu();
  } 
  else {
    Serial.println("Perintah tidak dikenal!");
  }
}

// ==========================================
// FUNGSI PENGGERAK RELAY (THE MUSCLES)
// ==========================================
void nyalakanLampu() {
  digitalWrite(PIN_RELAY, LOW); // Relay Active LOW
  matrix.loadFrame(ikonNyala);
  Serial.println("STATUS: Lampu NYALA");
}

void matikanLampu() {
  digitalWrite(PIN_RELAY, HIGH); // Relay Active LOW
  matrix.loadFrame(ikonMati);
  Serial.println("STATUS: Lampu MATI");
}
