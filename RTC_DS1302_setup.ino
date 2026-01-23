#include <ThreeWire.h>  
#include <RtcDS1302.h>

// Definisi Pin (IO/DAT, SCLK/CLK, CE/RST)
ThreeWire myWire(7, 6, 8); 
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
    Serial.begin(9600);
    Rtc.Begin();

    // Set tanggal & waktu spesifik secara manual
    // Format: Tahun, Bulan, Tanggal, Jam, Menit, Detik
    RtcDateTime manualTime = RtcDateTime(2026, 1, 23, 10, 30, 0); 
    Rtc.SetDateTime(manualTime); 
    
    Serial.println("Waktu berhasil di-set!");
}

void loop() {
    // Biarkan kosong
}
