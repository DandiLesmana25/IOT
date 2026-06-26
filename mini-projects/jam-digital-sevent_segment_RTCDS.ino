
// include library modul RTC
#include <ThreeWire.h>  
#include <RtcDS1302.h>
// library sevent segment
#include <TM1637Display.h>

// --- KONFIGURASI PIN ---            
// Pin TM1637
#define CLK_DISPLAY 2
#define DIO_DISPLAY 3

// Pin RTC DS1302
#define PIN_DAT 7
#define PIN_CLK 6
#define PIN_RST 8

// --- INISIALISASI OBJEK ---
TM1637Display display(CLK_DISPLAY, DIO_DISPLAY);
ThreeWire myWire(PIN_DAT, PIN_CLK, PIN_RST);
RtcDS1302<ThreeWire> Rtc(myWire);

// --- VARIABEL ---
bool colonState = false; // Status titik dua (Nyala/Mati)

void setup() {
  Serial.begin(9600);

  // 1. Setup Display
  display.setBrightness(0x0f); // Kecerahan Maksimal (0x0f)
  display.clear();

  // 2. Setup RTC
  Rtc.Begin();

  // LOGIKA "AUTO-SET" JAM 
  // Cek apakah data jam di modul valid?
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); // Ambil waktu laptop saat compile
  
  if (!Rtc.IsDateTimeValid()) {
    Serial.println("RTC Hilang Ingatan! Setting ulang ke waktu laptop...");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC terkunci. Membuka kunci...");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC belum jalan. Menjalankan...");
    Rtc.SetIsRunning(true);
  }


}

void loop() {
  // 1. Ambil Waktu Sekarang
  RtcDateTime now = Rtc.GetDateTime();
  
  int jam = now.Hour();
  int menit = now.Minute();
  int detik = now.Second();

  // 2. Format Data untuk TM1637
  // Rumus: (Jam * 100) + Menit. 
  // Contoh: Jam 12, Menit 30 -> 12 * 100 + 30 = 1230.
  int displayTime = (jam * 100) + menit;

  // 3. Logika Kedip Titik Dua (Colon Blink)
  // 0x40 adalah kode HEX untuk menyalakan titik dua tengah (0b01000000)
  // Operator Ternary: Jika detik genap -> Nyala (0     x40), Jika ganjil -> Mati (0)
  uint8_t dots = (detik % 2 == 0) ? 0x40 : 0; 

  // 4. Tampilkan ke Layar
  // Parameter: (Angka, TitikDua, LeadingZero?, Posisi)
  display.showNumberDecEx(displayTime, dots, true);

  // Debugging di Serial Monitor (Opsional)
  Serial.print(jam);
  Serial.print(":");
  Serial.println(menit);
  
  // Delay kecil agar responsif     
  delay(200); 
}
