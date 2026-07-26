#include <WiFiS3.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Memulai pemindaian WiFi sekitar...");
  int jumlahJaringan = WiFi.scanNetworks();

  if (jumlahJaringan == -1) {
    Serial.println("Tidak ada modul WiFi yang terdeteksi.");
    while (true);
  }

  Serial.print("Jumlah jaringan ditemukan: ");
  Serial.println(jumlahJaringan);

  for (int i = 0; i < jumlahJaringan; i++) {
    // Membungkus SSID dengan [ ] untuk mendeteksi spasi tersembunyi
    Serial.print("Nama WiFi: [");
    Serial.print(WiFi.SSID(i));
    Serial.print("] | Kekuatan Sinyal: ");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm");
  }
}

void loop() {}