#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
Servo gateServo;

// pin
int servoPin = 6;
int buzzerPin = 7;

// UID kartu yang diizinkan (contoh)
byte allowedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF};

void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  gateServo.attach(servoPin);
  pinMode(buzzerPin, OUTPUT);

  // posisi awal portal (tutup)
  gateServo.write(0);

  Serial.println("Tempelkan Kartu RFID...");
}

void loop() {
  // cek apakah ada kartu
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  bool akses = true;

  // bandingkan UID
  for (byte i = 0; i < 4; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    if (rfid.uid.uidByte[i] != allowedUID[i]) {
      akses = false;
    }
  }
  Serial.println();

  if (akses) {
    Serial.println("Akses DITERIMA");

    digitalWrite(buzzerPin, HIGH);
    gateServo.write(90);   // buka portal
    delay(2000);

    digitalWrite(buzzerPin, LOW);
    gateServo.write(0);    // tutup portal
  } else {
    Serial.println("Akses DITOLAK");

    // bunyi buzzer error
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
  }

  // hentikan komunikasi kartu
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
