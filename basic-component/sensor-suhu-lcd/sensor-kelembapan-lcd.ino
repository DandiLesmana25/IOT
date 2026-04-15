#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2        // Pin digital yang terhubung ke sensor DHT
#define DHTTYPE DHT11   // Tipe sensor DHT11, bisa diganti ke DHT22 jika memakai DHT22

// Inisialisasi sensor DHT dan LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address 0x27 untuk layar 16x2

void setup() {
  // Serial.begin(9600);
  dht.begin();
  lcd.init();       // Menginisialisasi LCD
  lcd.backlight();  // Menyalakan backlight LCD
}

void loop() {
  // Tunggu beberapa detik antar pengukuran
  delay(2000);

  // Membaca kelembaban
  float h = dht.readHumidity();
  // Membaca suhu dalam Celsius
  float t = dht.readTemperature();

  // Periksa apakah pembacaan gagal dan keluar jika ada kegagalan
  if (isnan(h) || isnan(t)) {
    // Serial.println("Gagal membaca dari sensor DHT!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    return;
  }

  // Menampilkan data di Serial Monitor
  // Serial.print("Kelembaban: ");
  // Serial.print(h);
  // Serial.print("%  Suhu: ");
  // Serial.print(t);
  // Serial.println("°C");

  // Menampilkan data di LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Suhu: ");
  lcd.print(t);
  lcd.print(" C");
}
