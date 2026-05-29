#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;


void setup() {
  Serial.begin(115200);
  
  // Inisialisasi matrix
  matrix.begin();

  // Pengaturan awal untuk grafis
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);     // Set warna LED menjadi menyala (putih/aktif)
  matrix.textScrollSpeed(100);   // Atur kecepatan scroll (semakin besar = semakin lambat)
  matrix.textFont(Font_5x7);
  matrix.endDraw();
}


void loop() {
  // Mulai proses menggambar frame baru
  matrix.beginDraw();
  
  // Bersihkan layar dari teks/gambar sebelumnya
  matrix.clear(); 
  
  // Konfigurasi posisi dan warna teks
  // Parameter: (X awal, Y awal, warna r, warna g, warna b)
  // Catatan: Karena matrix R4 hanya satu warna (merah), kita isi nilai warna dengan 0xFFFFFF (aktif)
  matrix.beginText(0, 1, 0xFFFFFF); 
  
  // Masukkan teks yang ingin ditampilkan
  // Tips: Tambahkan spasi di awal dan akhir agar transisi scroll terlihat mulus
  matrix.print("  Halo Dandi Ganteng!  ");
  
  // Akhiri pengaturan teks dan setel arah pergerakan (SCROLL_LEFT)
  matrix.endText(SCROLL_LEFT); 
  
  // Eksekusi dan tampilkan ke LED Matrix
  matrix.endDraw();
}