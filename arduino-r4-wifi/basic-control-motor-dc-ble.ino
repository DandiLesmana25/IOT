#include <ArduinoBLE.h>

// 1. DEFINISI PIN KONTROL L298N
const int pinENA = 5; // Pin PWM untuk mengatur kecepatan motor
const int pinIN1 = 3; // Pin kontrol arah jalan 1
const int pinIN2 = 4; // Pin kontrol arah jalan 2

// 2. STRUKTUR BLE (Buku Menu)
BLEService motorService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Kategori Menu

// Item Menu: Perintah Motor (Menerima tipe data Byte untuk angka 0, 1, atau 2)
BLEByteCharacteristic motorCharacteristic("19b10004-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);
  
  // Konfigurasi semua pin kontrol sebagai Output
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  
  // Pastikan motor dalam kondisi berhenti total saat awal dinyalakan
  berhentiMotor();

  // Jalankan modul BLE
  if (!BLE.begin()) {
    Serial.println("Gagal mengaktifkan modul Bluetooth!");
    while (1);
  }

  // Atur identitas Bluetooth R4
  BLE.setLocalName("Kemudi_Motor");
  BLE.setAdvertisedService(motorService);

  // Masukkan item kontrol ke dalam kategori menu
  motorService.addCharacteristic(motorCharacteristic);
  BLE.addService(motorService);

  // Set nilai awal di aplikasi HP = 0 (Berhenti)
  motorCharacteristic.writeValue(0);

  // Mulai pancarkan sinyal
  BLE.advertise();
  Serial.println("BLE Aktif! Menunggu perintah gerakan dari HP...");
}

void loop() {
  // Tunggu smartphone (Central) terhubung
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Terkoneksi ke handphone: ");
    Serial.println(central.address());

    // Selama handphone masih terhubung
    while (central.connected()) {
      
      // PERIKSA APAKAH ADA PERINTAH BARU DARI HP
      if (motorCharacteristic.written()) {
        
        int perintahDiterima = motorCharacteristic.value();
        Serial.print("Menerima kode perintah: ");
        Serial.println(perintahDiterima);

        // LOGIKA EKSEKUSI GERAKAN
        if (perintahDiterima == 1) {
          Serial.println("-> AKSI: Motor Berputar MAJU");
          majuMotor(200); // Jalankan dengan kecepatan PWM 200 (Skala 0-255)
        } 
        else if (perintahDiterima == 2) {
          Serial.println("-> AKSI: Motor Berputar MUNDUR");
          mundurMotor(200); // Jalankan dengan kecepatan PWM 200
        } 
        else {
          Serial.println("-> AKSI: Motor BERHENTI");
          berhentiMotor();
        }
      }
      
    } // Akhir dari while connected

    // Proteksi Keamanan: Jika HP putus koneksi secara tiba-tiba, matikan motor!
    Serial.println("Handphone terputus. Menghentikan motor demi keamanan.");
    berhentiMotor();
  }
}

// ==========================================
// FUNGSI-FUNGSI PENGGERAK MOTOR (CLEAN CODE)
// ==========================================

void majuMotor(int kecepatan) {
  analogWrite(pinENA, kecepatan); // Kirim sinyal PWM untuk mengatur kecepatan
  digitalWrite(pinIN1, HIGH);     // Kombinasi HIGH-LOW membuat motor berputar maju
  digitalWrite(pinIN2, LOW);
}

void mundurMotor(int kecepatan) {
  analogWrite(pinENA, kecepatan); // Kirim sinyal PWM untuk mengatur kecepatan
  digitalWrite(pinIN1, LOW);      // Kombinasi LOW-HIGH membalik arah putaran motor
  digitalWrite(pinIN2, HIGH);
}

void berhentiMotor() {
  analogWrite(pinENA, 0);         // Set kecepatan ke 0
  digitalWrite(pinIN1, LOW);      // Putus semua aliran listrik ke motor
  digitalWrite(pinIN2, LOW);
}
