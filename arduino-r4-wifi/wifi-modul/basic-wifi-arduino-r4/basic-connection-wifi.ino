#include <WiFiS3.h> // Library wajib untuk WiFi pada Arduino R4

// 1. MASUKKAN KREDENSIAL WIFI
// Minta siswa mengganti teks di bawah sesuai WiFi masing-masing
// char ssid[] = "RUANG BELAJAR";     
char ssid[] = "RUANG BELAJAR";     
char pass[] = "Ft020202*"; 

// Variabel untuk menyimpan status jaringan
int statusJaringan = WL_IDLE_STATUS; 

void setup() {
  Serial.begin(115200);
  while (!Serial); // Tunggu Serial Monitor dibuka

  Serial.println("--- PENCARIAN JARINGAN WIFI ---");

  // 2. PROSES MENGHUBUNGKAN (LOOPING)
  // Program akan terus mencoba terhubung selama statusnya BELUM "WL_CONNECTED"
  while (statusJaringan != WL_CONNECTED) {
    Serial.print("Mencoba terhubung ke SSID: ");
    Serial.println(ssid);
    
    // Perintah untuk mulai masuk ke jaringan WiFi
    statusJaringan = WiFi.begin(ssid, pass);

    // Tunggu 5 detik sebelum mencoba lagi jika gagal
    delay(5000);
  }

  // 3. JIKA BERHASIL TERHUBUNG
  Serial.println("\n=================================");
  Serial.println("SUKSES! Arduino Sudah Terhubung.");
  Serial.println("=================================");

  // CETAK IP ADDRESS (NOMOR MEJA ARDUINO)
  // Fungsi WiFi.localIP() akan mengambil alamat IP yang diberikan oleh Router
  IPAddress ip = WiFi.localIP();
  
  Serial.print("Alamat IP Address Arduino kamu: ");
  Serial.println(ip);
  Serial.println("---------------------------------");
}

void loop() {
  // Di kode dasar ini, void loop dibiarkan kosong karena kita 
  // hanya perlu menghubungkan WiFi sekali saja di awal (void setup)
}