#include <WiFiS3.h>

// --- KONFIGURASI WIFI ---
char ssid[] = "Wfdandilesmana"; 
char pass[] = "123123123"; 

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Gagal mendeteksi modul WiFi!");
    while (true);
  }

  Serial.print("Menghubungkan ke jaringan: ");
  Serial.println(ssid);
  
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  server.begin();
  Serial.println("Berhasil terhubung ke WiFi!");
  
  IPAddress ip = WiFi.localIP();
  Serial.print("API Server Aktif di IP: http://");
  Serial.println(ip);
}

// Fungsi bantuan untuk mengirim respons JSON yang sesuai standar REST API
void sendJsonResponse(WiFiClient& client, bool ledStatus) {
  int sensorValue = analogRead(A0);
  
  // 1. Header HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  // CRITICAL: Mengizinkan request dari file lokal komputer (CORS bypass)
  client.println("Access-Control-Allow-Origin: *"); 
  client.println("Connection: close");
  client.println(); // Baris kosong pembatas
  
  // 2. Body (JSON Format)
  client.print("{\"led\": ");
  client.print(ledStatus ? "true" : "false");
  client.print(", \"sensor\": ");
  client.print(sensorValue);
  client.println("}");
}

void loop() {
  WiFiClient client = server.available(); 

  if (client) {
    String currentLine = ""; 
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n') {
          // Jika request HTTP selesai, tetapi ini bukan endpoint valid, balas 404 atau default status
          if (currentLine.length() == 0) {
            // Jika ada request tanpa parameter yang ditangkap, default kirim status
            sendJsonResponse(client, digitalRead(LED_BUILTIN) == HIGH);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // --- ROUTING API ENDPOINTS ---
        
        // Endpoint 1: Menyalakan LED
        if (currentLine.endsWith("GET /api/led/on")) {
          digitalWrite(LED_BUILTIN, HIGH); 
          Serial.println("API Request: Turn LED ON");
          sendJsonResponse(client, true);
          break; // Selesai membalas, keluar loop
        }
        
        // Endpoint 2: Mematikan LED
        if (currentLine.endsWith("GET /api/led/off")) {
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("API Request: Turn LED OFF");
          sendJsonResponse(client, false);
          break;
        }
        
        // Endpoint 3: Menarik Status Saat Ini (Polling)
        if (currentLine.endsWith("GET /api/status")) {
          sendJsonResponse(client, digitalRead(LED_BUILTIN) == HIGH);
          break;
        }
      }
    }
    client.stop();
  }
}
