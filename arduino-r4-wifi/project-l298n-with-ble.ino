#include <ArduinoBLE.h>
#include <Arduino_LED_Matrix.h>

// --- INISIALISASI LED MATRIX ---
ArduinoLEDMatrix matrix;

// Array Animasi LED Matrix (Frame)
const uint32_t ikonMaju[] = {
  0x018003c0, 0x07e00ff0, 0x01800180
};
const uint32_t ikonMundur[] = {
  0x01800180, 0x0ff007e0, 0x03c00180
};
const uint32_t ikonBerhenti[] = {
  0x0ff00810, 0x08100810, 0x08100ff0
};

// --- INISIALISASI BLE ---
// Membuat custom UUID untuk service dan characteristic
BLEService carService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEByteCharacteristic commandChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// --- DEFINISI PIN MOTOR ---
#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 3
#define IN3 5
#define IN4 4

int speedDefault = 200; // Kecepatan jelajah

void setup() {
  Serial.begin(9600);

  // Setup Pin Motor
  pinMode(ENA, OUTPUT);   
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 

  // Setup LED Matrix
  matrix.begin();
  matrix.loadFrame(ikonBerhenti); // Tampilan awal

  // Setup BLE
  if (!BLE.begin()) {
    Serial.println("Gagal memulai modul BLE!");
    while (1);
  }

  BLE.setLocalName("RobotCar_R4"); // Nama robot yang muncul di HP
  BLE.setAdvertisedService(carService);
  carService.addCharacteristic(commandChar);
  BLE.addService(carService);

  commandChar.writeValue(0);
  BLE.advertise();

  Serial.println("Robot Siap! Silakan koneksikan via Bluetooth.");
}

void loop() {
  // Tunggu koneksi dari HP (Central Device)
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Terhubung ke: ");
    Serial.println(central.address());

    // Selama HP terhubung, baca perintah yang dikirim
    while (central.connected()) {
      if (commandChar.written()) {
        char perintah = (char)commandChar.value();
        eksekusiPerintah(perintah);
      }
    }
    
    // Jika koneksi terputus, otomatis berhenti
    Serial.println("Koneksi terputus.");
    berhenti();
    matrix.loadFrame(ikonBerhenti);
  }
}

// ==========================================
// FUNGSI PENGOLAH PERINTAH
// ==========================================

void eksekusiPerintah(char cmd) {
  switch (cmd) {
    case 'F':
      maju(speedDefault);
      matrix.loadFrame(ikonMaju);
      break;
    case 'B':
      mundur(speedDefault);
      matrix.loadFrame(ikonMundur);
      break;
    case 'L':
      belokKiri(speedDefault);
      // Opsional: Buat ikon panah kiri
      break;
    case 'R':
      belokKanan(speedDefault);
      // Opsional: Buat ikon panah kanan
      break;
    case 'S':
      berhenti();
      matrix.loadFrame(ikonBerhenti);
      break;
  }
}

// ==========================================
// FUNGSI PENGGERAK MOTOR (MUSCLES)
// ==========================================

void maju(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void mundur(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void belokKiri(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void belokKanan(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void berhenti() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}