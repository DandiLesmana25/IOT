/*
 * PROJECT: Termometer Digital Ruangan
 * HARDWARE: Arduino Uno + LCD 16x2 I2C + DHT11
 * LOKASI PIN: DHT11 di Pin 2, SDA di A4, SCL di A5
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// --- KONFIGURASI SENSOR ---
#define DHTPIN 2          // Pin Data DHT11 dicolok ke Pin 2 digital
#define DHTTYPE DHT11     // Jenis sensor (Ganti ke DHT22 jika pakai yang putih)
DHT dht(DHTPIN, DHTTYPE);

// --- KONFIGURASI LCD ---
// Alamat I2C umumnya 0x27 atau 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- MEMBUAT IKON DERAJAT CUSTOM ---
// Array biner untuk menggambar lingkaran kecil di pojok kiri atas (8x5 pixel)
byte derajatCustom[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  
  // 1. Inisialisasi Sensor DHT
  dht.begin();
  
  // 2. Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Mendaftarkan ikon derajat ke memori LCD di ID nomor 0
  lcd.createChar(0, derajatCustom); 

  // Menampilkan layar pembuka (Splash Screen)
  lcd.setCursor(0, 0);
  lcd.print("Sistem Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Memuat Data...");
  
  // PENTING: Beri waktu DHT11 untuk pemanasan (minimal 2 detik)
  delay(2000); 
  lcd.clear();
}

void loop() {
  // --- BACA DATA SENSOR ---
  float suhu = dht.readTemperature(); // Baca Celcius
  float kelembaban = dht.readHumidity();  // Baca %

  // --- CEK ERROR (Validasi Data) ---
  if (isnan(suhu) || isnan(kelembaban)) {
    lcd.setCursor(0, 0);
    lcd.print("Error Sensor! ");
    lcd.setCursor(0, 1);
    lcd.print("Cek Kabel Data");
    return; // Berhenti di sini, ulang loop dari awal
  }

  // --- TAMPILKAN KE LCD ---
  
  // Baris 1: Suhu
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(suhu, 1);       // Tampilkan dengan 1 angka di belakang koma (misal: 28.5)
  lcd.write(0);             // Panggil custom character derajat (ID 0)
  lcd.print("C  ");         // Spasi ekstra untuk menimpa sisa karakter lama

  // Baris 2: Kelembaban
  lcd.setCursor(0, 1);
  lcd.print("Lembab: ");
  lcd.print(kelembaban, 1);
  lcd.print(" %  ");

  // DHT11 sangat lambat, jangan refresh terlalu cepat.
  // Delay 2 detik adalah batas aman agar sensor tidak hang.
  delay(2000); 
}
