#include <ArduinoBLE.h>
#include "ArduinoGraphics.h"     // Library pembuat teks
#include "Arduino_LED_Matrix.h"  // Library pengontrol layar R4

// 1. PERSIAPAN BLE (Kotak Surat)
BLEService customService("FFF0"); 
BLEByteCharacteristic rxCharacteristic("FFF1", BLERead | BLEWrite);

// 2. PERSIAPAN LED MATRIX
ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  
  // Nyalakan layar LED Matrix R4
  matrix.begin();
  
  // Atur jenis huruf (font) standar untuk layar Matrix
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF); // Warna LED menyala penuh
  matrix.textFont(Font_5x7); // Ukuran teks standar
  matrix.endDraw();

  // Nyalakan Modul BLE
  if (!BLE.begin()) {
    Serial.println("Gagal menyalakan modul BLE!");
    while (1); 
  }

  // Atur Identitas Restoran / Bluetooth
  BLE.setLocalName("R4_Matrix"); 
  BLE.setAdvertisedService(customService); 
  customService.addCharacteristic(rxCharacteristic); 
  BLE.addService(customService); 

  rxCharacteristic.writeValue(0); // Nilai awal kotak surat

  BLE.advertise();
  Serial.println("Bluetooth dan Layar aktif! Menunggu koneksi...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Terkoneksi dengan HP: ");
    Serial.println(central.address());

    // Beri tanda senyum statis saat HP baru saja terhubung
    byte senyum[8][12] = {
      { 0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,1,0,0,0,0,0,0,1,0,0 },
      { 0,0,1,0,0,0,0,0,0,1,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,1,0,0,0,0,0,0,0,0,1,0 },
      { 0,0,1,1,1,1,1,1,1,1,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0 }
    };
    matrix.renderBitmap(senyum, 8, 12);

    while (central.connected()) {
      
      // LOGIKA UTAMA: JIKA ADA DATA MASUK DARI HP
      if (rxCharacteristic.written()) {
        
        byte dataMasuk = rxCharacteristic.value(); 
        
        // JIKA HP MENGIRIM ANGKA '1' (Bisa berupa angka atau karakter teks)
        if (dataMasuk == 1 || dataMasuk == '1') {
          Serial.println("Perintah diterima: 1");
          tampilkanTeksBerjalan("HALO"); // Panggil fungsi pembuat teks
        } 
        
        // JIKA HP MENGIRIM ANGKA '0'
        else if (dataMasuk == 0 || dataMasuk == '0') {
          Serial.println("Perintah diterima: 0");
          tampilkanTeksBerjalan("OFF");
        }
        
        // Kembalikan layar ke gambar senyum setelah teks selesai berjalan
        matrix.renderBitmap(senyum, 8, 12);
      }
      
    } 

    Serial.println("HP terputus.");
    matrix.clear(); // Bersihkan layar saat HP putus untuk menghemat daya
  }
}

// ==========================================
// FUNGSI KHUSUS: MEMBUAT ANIMASI TEKS BERJALAN
// ==========================================
void tampilkanTeksBerjalan(String teks) {
  // Hitung perkiraan panjang teks agar tahu kapan animasi harus berhenti
  int panjangTeks = teks.length() * 6; 
  
  // Geser teks dari koordinat paling kanan (12) ke paling kiri (-panjangTeks)
  for (int pos = 12; pos >= -panjangTeks; pos--) {
    matrix.beginDraw();
    matrix.clear(); // Hapus frame sebelumnya
    
    matrix.beginText(pos, 1, 0xFFFFFF); // Tulis huruf di posisi X=pos, Y=1
    matrix.print(teks);
    matrix.endText();
    
    matrix.endDraw(); // Tampilkan ke layar fisik
    
    delay(70); // Kecepatan jalannya teks (semakin kecil = semakin cepat)
  }
}
