#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  
  // Cek address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.display();
}

void loop() {
  // Simulasi data sensor
  int suhu = 26;
  int kelembaban = 75;

  display.clearDisplay(); // 1. Hapus layar dulu

  // --- Header ---
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("IOT DASHBOARD");
  display.drawLine(0, 10, 128, 10, WHITE); // Garis pemisah

  // --- Data Utama ---
  display.setTextSize(2); // Ukuran besar untuk angka
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(suhu);
  display.setTextSize(1); // Ukuran kecil untuk satuan
  display.cp437(true);    // Enable simbol khusus
  display.write(248);     // Kode ASCII untuk simbol Derajat (°)
  display.print("C");

  // --- Data Kedua ---
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("Humidity: ");
  display.print(kelembaban);
  display.print("%");

  display.display(); // 2. Tampilkan semua perubahan
  
  delay(2000);
}
