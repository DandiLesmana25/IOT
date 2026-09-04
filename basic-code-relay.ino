// Define the Arduino pin connected to the relay's signal (IN) pin
const int RELAY_PIN = 8; 

void setup() {
  // Configure the relay pin as an OUTPUT
  pinMode(RELAY_PIN, OUTPUT);
  
  // Optional: Set initial state to OFF when the Arduino starts
  digitalWrite(RELAY_PIN, LOW); 
  Serial.begin(9600);
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH); // Turn the relay ON
  Serial.println("pin high => Saklar TERTUTUP (ON) magnet menyala");
  delay(2000);                   // Wait for 2 seconds (2000 milliseconds)

  digitalWrite(RELAY_PIN, LOW);  // Turn the relay OFF
    Serial.println("pin LOW =>  Magnet mati ➔ Saklar TERBUKA (OFF) ➔ Relay non-aktif.");
  delay(2000);                   // Wait for 2 seconds
}
