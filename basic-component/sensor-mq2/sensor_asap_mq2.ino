int mq2Pin = A0;
int batasAsap = 300;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int nilaiAsap = analogRead(mq2Pin);

  Serial.println(nilaiAsap);

  if (nilaiAsap > batasAsap) {
    Serial.println("Asap terdeteksi!");
  } else {
    Serial.println("Udara aman");
  }

  delay(10000);
}


