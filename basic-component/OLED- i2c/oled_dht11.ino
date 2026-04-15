/*
 * DHT11 dengan OLED 0.96 Inch I2C
 * Tampilkan suhu & kelembaban di OLED, dengan logika sederhana
 */

#include <DHT.h>                // Library DHT11
#include <Adafruit_GFX.h>       // Library GFX untuk grafis
#include <Adafruit_SSD1306.h>   // Library OLED SSD1306

// Deklarasi untuk DHT11
#define DHTPIN 2                // Pin data DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Deklarasi untuk OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // I2C, no reset pin

// Variabel (dari materi sebelumnya)
float suhuRuangan = 0.0;        // Float untuk suhu
float kelembaban = 0.0;         // Float untuk kelembaban
boolean suhuTinggi = false;     // Boolean untuk status suhu

void setup() {
  Serial.begin(9600);           // Opsional untuk debug
  dht.begin();                  // Inisialisasi DHT
  
  // Inisialisasi OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal start"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sensor Siap!");
  display.display();
  delay(2000);                  // Tampilkan pesan awal 2 detik
}

void loop() {
  // Baca data DHT (assignment operator)
  suhuRuangan = dht.readTemperature();
  kelembaban = dht.readHumidity();
  
  // Cek error (logical OR)
  if (isnan(suhuRuangan) || isnan(kelembaban)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Gagal baca sensor!");
    display.display();
  } else {
    // Logika dengan operator (comparison & assignment)
    suhuTinggi = (suhuRuangan > 30);  // Cek suhu > 30
    
    // Tampilkan ke OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Suhu: ");
    display.print(suhuRuangan);       // Print variabel float
    display.println(" C");
    
    display.setCursor(0, 20);         // Baris baru
    display.print("Kelembaban: ");
    display.print(kelembaban);
    display.println(" %");
    
    display.setCursor(0, 40);
    if (suhuTinggi) {                 // If dengan boolean
      display.println("Suhu Tinggi!");
    } else {
      display.println("Suhu Normal");
    }
    
    display.display();                // Update layar
  }
  
  delay(2000);  // Tunggu 2 detik (DHT11 butuh waktu)
}
