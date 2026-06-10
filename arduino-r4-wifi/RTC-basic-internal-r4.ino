#include "RTC.h"

void setup() {
  Serial.begin(115200);
  
  // 1. Jalankan komponen RTC
  RTC.begin();

  // 2. Set waktu awal (Jam, Menit, Detik) saat program ini di-upload
  // Format: RTCTime namaVariabel(Tanggal, Bulan, Tahun, Jam, Menit, Detik, DayOfWeek, SaveLight)
  // Contoh: Set ke Jam 05:15:00
  RTCTime startTime(11, Month::JUNE, 2026, 5, 15, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);
  
  RTC.setTime(startTime);
}

void loop() {
  // 3. Buat wadah untuk mengambil waktu saat ini
  RTCTime currentTime;
  
  // 4. Ambil waktu sekarang dari RTC dimasukkan ke wadah
  RTC.getTime(currentTime);
  
  // 5. Tampilkan ke Serial Monitor
  Serial.print("Jam saat ini: ");
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinute());
  Serial.print(":");
  Serial.println(currentTime.getSecond());
  
  delay(1000); // Update setiap 1 detik
}
