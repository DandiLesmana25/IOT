void setup() {
  // Inisialisasi LED bawaan
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Memulai komunikasi serial dengan kecepatan tinggi
  // R4 bisa menangani komunikasi data yang sangat cepat!
  Serial.begin(115200); 
  
  // Tunggu sebentar agar Serial Monitor siap
  delay(1000);
  Serial.println("Halo dari Arduino Uno R4 WiFi!");
  Serial.println("Mesin siap dijalankan!");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // Nyalakan LED
  Serial.println("LED Menyala");
  delay(500);                       
  
  digitalWrite(LED_BUILTIN, LOW);   // Matikan LED
  Serial.println("LED Mati");
  delay(500);                       
}