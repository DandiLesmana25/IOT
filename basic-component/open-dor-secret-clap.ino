// Pin connections
int soundSensor = 8;
int ledPin = 7;

// Program variables
int knockCount = 0;
int requiredKnocks = 5;

int lastSoundState = 0;
int currentSoundState = 0;

void setup() {
pinMode(soundSensor, INPUT);
pinMode(ledPin, OUTPUT);
Serial.begin(9600);
digitalWrite(ledPin, HIGH); // Pastikan LED mati saat awal

Serial.println("system nyalakan lampu rahasia siapp");
}

void loop() {
// Membaca sensor suara
currentSoundState = digitalRead(soundSensor);

// Menentukan jika ketukan terdeteksi
if (lastSoundState == 0 && currentSoundState == 1) {
knockCount++;
Serial.print("Knock detected: ");
Serial.println(knockCount);
delay(250); // Debounce sederhana agar 1 ketukan tidak dihitung ganda
}

// Memeriksa apakah jumlah ketukan sudah tercapai
if (knockCount == requiredKnocks) {
digitalWrite(ledPin, LOW); // Nyalakan LED (buka kunci)
delay(5000);               // Tahan selama 5 detik
digitalWrite(ledPin, HIGH);  // Matikan LED kembali
knockCount = 0;             // Reset hitungan
}

// Update status suara terakhir
lastSoundState = currentSoundState;
}
