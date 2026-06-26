/*
 * PROJECT: Smart Visitor Counter
 * HARDWARE: Arduino Uno + IR Sensor (FC-51) + LCD I2C + Active Buzzer
 * LOGIC: State Change Detection (Mencegah multiple-counting)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- KONFIGURASI PIN ---
#define PIN_IR 2
#define PIN_BUZZER 3

// --- KONFIGURASI LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // Coba 0x3F jika 0x27 tidak muncul

// --- VARIABEL GLOBAL ---
int jumlahPengunjung = 0;
int statusSensorSaatIni = 0;
int statusSensorSebelumnya = HIGH; // Awalnya HIGH (Tidak ada objek)

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_IR, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  // Matikan buzzer di awal
  digitalWrite(PIN_BUZZER, LOW); 

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Tampilan Awal Layar
  lcd.setCursor(0, 0);
  lcd.print("Sistem Penghitung");
  lcd.setCursor(0, 1);
  lcd.print("Total: 0 Orang");
}

void loop() {
  // 1. Baca data dari sensor IR
  // Ingat: Modul IR FC-51 bersifat Active LOW (LOW = Ada Objek, HIGH = Kosong)
  statusSensorSaatIni = digitalRead(PIN_IR);

  // 2. LOGIKA STATE CHANGE (Deteksi Perubahan)
  // Cek apakah status saat ini BERBEDA dengan status sebelumnya?
  if (statusSensorSaatIni != statusSensorSebelumnya) {
    
    // Jika berubah, cek apakah perubahannya adalah menjadi LOW (Objek datang)?
    if (statusSensorSaatIni == LOW) {
      
      jumlahPengunjung++;
      
      // Update tampilan LCD
      updateLCD();
      
      // Bunyikan Buzzer (Bunyi "Beep" pendek)
      digitalWrite(PIN_BUZZER, HIGH);
      delay(100); // Buzzer nyala 100 milidetik
      digitalWrite(PIN_BUZZER, LOW);
      
      Serial.print("Pengunjung masuk. Total: ");
      Serial.println(jumlahPengunjung);
    } 
    else {
      // Logika saat objek pergi (kembali ke HIGH)
      // Kita tidak melakukan perhitungan, biarkan saja
      Serial.println("Pengunjung melewati sensor...");
    }
    
    //  Delay kecil untuk "Debouncing" (menghilangkan noise sinyal)
    delay(50);
  }

  // 3. Update status sebelumnya dengan status saat ini untuk loop berikutnya
  statusSensorSebelumnya = statusSensorSaatIni;
}

// --- FUNGSI CUSTOM ---
void updateLCD() {
  // Hanya menghapus baris kedua agar layar tidak berkedip (flicker) parah
  lcd.setCursor(7, 1); 
  lcd.print("         "); // Menimpa dengan spasi kosong untuk hapus angka lama
  
  lcd.setCursor(7, 1);
  lcd.print(jumlahPengunjung);
  lcd.print(" Orang");
}
