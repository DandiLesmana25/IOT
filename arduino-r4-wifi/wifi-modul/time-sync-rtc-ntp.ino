#include "WiFiS3.h"
#include "RTC.h" // Memanggil library RTC khusus bawaan Arduino R4

const char ssid[] = "Wifidandiganteng"; 
const char pass[] = "123123123";

// Zona Waktu: GMT+7 (WIB: Waktu Indonesia Barat)
// Karena 1 jam = 3600 detik, maka 7 jam = 7 x 3600 = 25200 detik
const int offsetTime = 25200; 

void setup() {
  Serial.begin(115200);

  // 1. Inisialisasi Modul RTC
  RTC.begin();
  
  // 2. Hubungkan ke WiFi
  Serial.print("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");

  // 3. Tarik waktu dari Server NTP dan sinkronkan ke RTC
  Serial.println("Meminta data waktu dari server NTP (Internet)...");
  
  // Fungsi khusus untuk mendapatkan waktu dari internet dengan penyesuaian GMT
  // auto ntpTime = WiFi.getTime(offsetTime); 
  unsigned long ntpTime = WiFi.getTime(); 
  

  if (ntpTime != 0) {
    // Jika berhasil, atur jam internal RTC agar sama dengan jam NTP
    ntpTime = ntpTime + offsetTime;

    RTCTime waktuSekarang(ntpTime);
    RTC.setTime(waktuSekarang);
    Serial.println("Berhasil menyinkronkan jam!");
  } else {
    Serial.println("Gagal terhubung ke server NTP. Coba periksa internet.");
  }
  
  // Pilihan: Di titik ini Anda sebenarnya bisa memutuskan WiFi 
  // menggunakan perintah WiFi.disconnect() jika ingin menghemat baterai,
  // karena RTC akan terus berdetak secara mandiri!
}

void loop() {
  // Buat objek penampung waktu
  RTCTime current;
  
  // Baca detik, menit, jam saat ini dari chip RTC
  RTC.getTime(current);
  
  // Cetak dalam format yang rapi: HH:MM:SS
  Serial.print("Jam Saat Ini -> ");
  Serial.print(current.getHour());
  Serial.print(":");
  Serial.print(current.getMinutes());
  Serial.print(":");
  Serial.println(current.getSeconds());
  
  // Cetak juga tanggalnya
  Serial.print("Tanggal -> ");
  Serial.print(current.getDayOfMonth());
  Serial.print("-");
  Serial.print(Month2int(current.getMonth())); // Konversi bulan ke angka
  Serial.print("-");
  Serial.println(current.getYear());
  
  Serial.println("-------------------------");
  
  delay(1000); // Tunggu 1 detik sebelum mencetak lagi
}
