#include <SoftwareSerial.h>

// RX Arduino (Konek ke TX SIM800L)
// TX Arduino (Konek ke RX SIM800L)
SoftwareSerial mySIM(10, 11); 

void setup() {
  Serial.begin(9600); // Komunikasi ke Laptop
  mySIM.begin(9600);  // Komunikasi ke SIM800L (Default baudrate biasanya 9600)
  
  Serial.println("SIM800L Ready...");
}

void loop() {
  // Jika ada data dari SIM800L, kirim ke Serial Monitor
  if (mySIM.available()) {
    Serial.write(mySIM.read());
  }
  
  // Jika kita ketik sesuatu di Serial Monitor, kirim ke SIM800L
  if (Serial.available()) {
    mySIM.write(Serial.read());
  }
}
