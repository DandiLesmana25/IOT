#include <TM1637Display.h>

#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

void setup() {
  display.setBrightness(7); // Kecerahan sedang
}

void loop() {
  // Misal Jam 12:30
  int jam = 11;
  int menit = 00;
  
  // Format waktu menjadi integer gabungan: 1230
  int waktu = (jam * 100) + menit;

  // --- LOGIKA KEDIP ---
  // Parameter ke-2: (0b01000000) atau (0x40) adalah kode untuk TITIK DUA di tengah
  // CATATAN: Tiap modul beda wiring titik dua-nya. 
  // Jika kode ini tidak nyala, coba 0x80.
  
  // Nyalakan Titik Dua
  display.showNumberDecEx(waktu, 0b01000000, true); 
  delay(500);
  
  // Matikan Titik Dua (Tampilkan angka saja)
  display.showNumberDecEx(waktu, 0, true);
  delay(500);
}
