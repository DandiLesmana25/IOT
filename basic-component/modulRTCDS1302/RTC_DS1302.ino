#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library LCD Frank de Brabander
#include <ThreeWire.h>  
#include <RtcDS1302.h>  // Library RTC by Makuna

// 1. Setup LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 2. Setup RTC (IO=7, CLK=6, RST=8)
ThreeWire myWire(7, 6, 8); 
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
    Serial.begin(9600);
    
    // Init LCD
    lcd.init();
    lcd.backlight();
    
    // Init RTC
    Rtc.Begin();
    
    // PENTING: Cek jika RTC error/baterai habis
    if (!Rtc.IsDateTimeValid()) {
        lcd.print("Error RTC!");
    }

    // Uncomment baris di bawah ini HANYA JIKA jam ngaco, lalu upload.
    // Setelah jam benar, comment lagi dan upload ulang.
    // RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // Rtc.SetDateTime(compiled);
}

void loop() {
    RtcDateTime now = Rtc.GetDateTime();
    
    // --- Tampilkan Jam (Baris 1) ---
    lcd.setCursor(0,0);
    lcd.print("Pukul: ");
    printDigits(now.Hour());
    lcd.print(":");
    printDigits(now.Minute());
    lcd.print(":");
    printDigits(now.Second());

    // --- Tampilkan Tanggal (Baris 2) ---
    lcd.setCursor(0,1);
    lcd.print("Tgl: ");
    printDigits(now.Day());
    lcd.print("/");
    printDigits(now.Month());
    lcd.print("/");
    lcd.print(now.Year());

    delay(1000);
}

// Function Helper biar angka < 10 ada '0' depannya
void printDigits(int digits){
    if(digits < 10)
        lcd.print("0");
    lcd.print(digits);
}
