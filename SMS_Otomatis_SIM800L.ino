#include <SoftwareSerial.h>

SoftwareSerial mySIM(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  mySIM.begin(9600);

  Serial.println("Menyiapkan SMS...");
  delay(10000); // Beri waktu 10 detik agar SIM800L dapat sinyal dulu!

  kirimSMS();
}

void loop() {
  // Kosong
}

void kirimSMS() {
  // 1. Set format SMS ke Text Mode
  mySIM.println("AT+CMGF=1"); 
  updateSerial();
  delay(500);

  // 2. Masukkan Nomor Tujuan (Gunakan kode negara +62)
  mySIM.println("AT+CMGS=\"+6281234567890\""); 
  updateSerial();
  delay(500);

  // 3. Tulis Isi Pesan
  mySIM.print("Halo Dandi, ini pesan percobaan dari Arduino!"); 
  updateSerial();
  delay(500);

  // 4. Kirim Karakter Penutup (CTRL+Z atau ASCII 26)
  mySIM.write(26); 
  delay(1000);
  
  Serial.println("SMS Terkirim (Semoga!)");
}

// Fungsi bantu debugging
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySIM.write(Serial.read());
  }
  while (mySIM.available()) {
    Serial.write(mySIM.read());
  }
}
