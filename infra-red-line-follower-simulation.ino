
#define SENSOR_KIRI 2
#define SENSOR_KANAN 3


void setup() {
  // Config Sensor sebagai INPUT
  pinMode(SENSOR_KIRI, INPUT);
  pinMode(SENSOR_KANAN, INPUT);

  
  Serial.begin(9600); // Untuk debugging sensor
}

void loop() {
  // Baca Data Sensor
  // Ingat: LOW = Putih (Pantul), HIGH = Hitam (Serap)
  int bacaKiri = digitalRead(SENSOR_KIRI);
  int bacaKanan = digitalRead(SENSOR_KANAN);

  // --- LOGIKA KONTROL ---
  
  // 1. KONDISI MAJU: Keduanya di atas Putih (Garis ada di tengah robot)
  if (bacaKiri == LOW && bacaKanan == LOW) {
    gerakMaju();
  }
  
  // 2. KONDISI BELOK KIRI: Sensor Kiri kena Hitam
  else if (bacaKiri == HIGH && bacaKanan == LOW) {
    belokKiri();
  }
  
  // 3. KONDISI BELOK KANAN: Sensor Kanan kena Hitam
  else if (bacaKiri == LOW && bacaKanan == HIGH) {
    belokKanan();
  }
  
  // 4. KONDISI STOP: Keduanya Hitam (Garis Finish)
  else if (bacaKiri == HIGH && bacaKanan == HIGH) {
    berhenti();
  }
}



void gerakMaju() {
  // simulais gerak maju
 Serial.println("majuuuuuuu");

}

void belokKiri() {
  // simulasi belok kiri
 Serial.println("Belok kirrrriii");
}

void belokKanan() {
  // simulasi belok kanan
 Serial.println("belok kanannnnnn");
}

void berhenti() {
 Serial.println("berhentiiiiii");
}


