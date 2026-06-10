#include "RTC.h"
#include "ArduinoGraphics.h" // Library untuk menulis teks
#include "Arduino_LED_Matrix.h" // Library layar R4

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  
  matrix.begin();
  RTC.begin();

  // Atur waktu awal (Bisa disesuaikan dengan jam laptop saat ini)
  RTCTime startTime(11, Month::JUNE, 2026, 5, 15, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(startTime);
  
  // Atur konfigurasi teks pada layar Matrix
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF); // Warna LED cerah
  matrix.textFont(Font_5x7);  // Ukuran font standar yang pas untuk layar 12x8
  matrix.endDraw();
}

void loop() {
  RTCTime currentTime;
  RTC.getTime(currentTime);

  // LOGIKA TANTANGAN: Membuat format string jam "05:15"
  String textJam = "";
  
  // Jika jam di bawah 10, tambahkan angka 0 di depannya
  if (currentTime.getHour() < 10) {
    textJam += "0";
  }
  textJam += String(currentTime.getHour());
  
  textJam += ":"; // Pembatas titik dua
  
  // Jika menit di bawah 10, tambahkan angka 0 di depannya
  if (currentTime.getMinutes() < 10) {
    textJam += "0";
  }
  textJam += String(currentTime.getMinutes());

  // PROSES MEMBUAT TEKS BERJALAN
  // Karena textJam (5 karakter) lebih lebar dari layar R4 (12 piksel), 
  // kita harus menggeser posisinya dari kanan ke kiri.
  
  int panjangTeks = textJam.length() * 6; // Estimasi lebar teks dalam piksel
  
  for (int pos = 12; pos >= -panjangTeks; pos--) {
    matrix.beginDraw();
    matrix.clear(); // Bersihkan layar lama
    
    matrix.beginText(pos, 1, 0xFFFFFF); // Mulai tulis di koordinat X=pos, Y=1
    matrix.print(textJam);
    matrix.endText();
    
    matrix.endDraw();
    delay(80); // Mengatur kecepatan jalannya teks (makin kecil makin cepat)
  }
}
