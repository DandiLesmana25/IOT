#include <WiFiS3.h>

// --- KONFIGURASI WIFI ---
// Ganti bagian ini dengan nama dan password WiFi Anda!
char ssid[] = "NAMA_WIFI_ANDA"; 
char pass[] = "PASSWORD_WIFI_ANDA"; 

int status = WL_IDLE_STATUS;
WiFiServer server(80); // Membuat server di port 80 (standar web)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); // Menggunakan LED internal bawaan (Pin 13)
  digitalWrite(LED_BUILTIN, LOW); // Pastikan LED mati saat awal

  // Cek apakah modul WiFi tersedia
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Gagal mendeteksi modul WiFi!");
    while (true); // Berhenti di sini jika modul rusak
  }

  // Proses menghubungkan ke WiFi
  Serial.print("Menghubungkan ke jaringan: ");
  Serial.println(ssid);
  
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000); // Tunggu 5 detik sebelum mencoba lagi jika gagal
  }

  // Jika berhasil terhubung
  server.begin(); // Jalankan server
  Serial.println("Berhasil terhubung ke WiFi!");
  
  // Menampilkan IP Address Arduino
  IPAddress ip = WiFi.localIP();
  Serial.print("Buka browser di HP/Laptop dan ketik alamat IP ini: http://");
  Serial.println(ip);
}

void loop() {
  // Mendengarkan jika ada client (browser) yang masuk
  WiFiClient client = server.available(); 

  if (client) {
    Serial.println("Ada Client/Browser yang mengakses!");
    String currentLine = ""; 
    
    // Selama client masih terhubung
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        // Jika kita menerima karakter newline (enter)
        if (c == '\n') {
          // Jika baris saat ini kosong, berarti request HTTP sudah selesai (akhir dari header)
          if (currentLine.length() == 0) {
            
            // 1. KIRIM HEADER HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println(); // Baris kosong pemisah antara header dan konten
            
            // 2. KIRIM KONTEN HTML & CSS
            client.println("<!DOCTYPE html><html><head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>Arduino R4 Dashboard</title>");
            
            // Desain CSS Dasar agar terlihat rapi di HP
            client.println("<style>");
            client.println("body { font-family: 'Segoe UI', Tahoma, sans-serif; background-color: #f8fafc; text-align: center; margin: 0; padding: 20px; color: #1e293b; }");
            client.println("h1 { color: #2563eb; font-size: 24px; }");
            client.println(".card { background: white; max-width: 400px; margin: 20px auto; padding: 25px; border-radius: 12px; box-shadow: 0 4px 6px rgba(0,0,0,0.05); }");
            client.println(".btn { display: inline-block; padding: 12px 24px; font-size: 18px; font-weight: bold; text-decoration: none; border-radius: 8px; color: white; margin: 10px 5px; transition: 0.2s; }");
            client.println(".btn-on { background-color: #10b981; }");
            client.println(".btn-off { background-color: #ef4444; }");
            client.println(".sensor-val { font-size: 40px; font-weight: bold; color: #3b82f6; margin: 10px 0; }");
            client.println(".refresh-link { color: #64748b; text-decoration: none; font-size: 14px; margin-top: 15px; display: inline-block; }");
            client.println("</style></head>");
            
            // 3. KIRIM BADAN WEB (BODY)
            client.println("<body>");
            client.println("<h1>Arduino R4 WiFi Web Server</h1>");
            
            // CARD 1: Kontrol LED
            client.println("<div class=\"card\">");
            client.println("<h3>Kontrol Lampu (LED Built-in)</h3>");
            
            // Tampilkan status lampu saat ini
            if(digitalRead(LED_BUILTIN) == HIGH) {
                client.println("<p>Status Saat Ini: <strong style='color:#10b981'>MENYALA</strong></p>");
            } else {
                client.println("<p>Status Saat Ini: <strong style='color:#ef4444'>MATI</strong></p>");
            }

            // Tombol Kontrol (Akan mengirimkan URL /H atau /L saat ditekan)
            client.println("<a href=\"/H\" class=\"btn btn-on\">Nyalakan</a>");
            client.println("<a href=\"/L\" class=\"btn btn-off\">Matikan</a>");
            client.println("</div>");
            
            // CARD 2: Bacaan Sensor
            client.println("<div class=\"card\">");
            client.println("<h3>Nilai Sensor Lokal (Pin A0)</h3>");
            
            // Membaca nilai pin Analog A0
            int sensorValue = analogRead(A0);
            
            client.print("<div class=\"sensor-val\">");
            client.print(sensorValue);
            client.println("</div>");
            client.println("<p style='color:#94a3b8; font-size:14px;'><i>(Nilai acak jika pin A0 dibiarkan kosong)</i></p>");
            
            // Tombol refresh yang mengarah ke home (/)
            client.println("<a href=\"/\" class=\"refresh-link\">&#8635; Refresh Nilai Sensor</a>");
            client.println("</div>");
            
            client.println("</body></html>");
            client.println(); // Akhir dari respons HTTP
            break; // Keluar dari loop while(client.connected())
          } else {
            // Jika kita mendapat newline tapi barisnya tidak kosong, clear barisnya
            currentLine = "";
          }
        } else if (c != '\r') {
          // Jika bukan karakter carriage return (\r), tambahkan karakter ke baris saat ini
          currentLine += c;
        }

        // LOGIKA KONTROL: Mengecek Request dari Browser
        // Jika pengguna menekan tombol "Nyalakan", browser akan mengakses IP_ARDUINO/H
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH); 
          Serial.println("Perintah: LED Dinyalakan dari Web");
        }
        // Jika pengguna menekan tombol "Matikan", browser akan mengakses IP_ARDUINO/L
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("Perintah: LED Dimatikan dari Web");
        }
      }
    }
    // Putuskan koneksi agar browser bisa selesai memuat
    client.stop();
    Serial.println("Client Terputus.\n");
  }
}
