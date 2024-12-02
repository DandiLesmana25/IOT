#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Replace with your network credentials
const char* ssid = "wifiname";
const char* password = "pwwifi";

// API endpoint to send data
const char* serverUrl = "http://172.20.10.3:3000/data-sensor";

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read humidity and temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  // Print data to serial monitor
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  // Send data to the API via HTTP POST
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client; // Create a WiFiClient object

    // Set up the HTTP request
    http.begin(client, serverUrl);  // Begin HTTP request with the WiFiClient and URL
    http.addHeader("Content-Type", "application/json");

    // Prepare JSON data
    String jsonData = "{\"temperature\": " + String(t, 4) + 
                      ", \"humidity\": " + String(h, 4) + 
                      "}";

    // Send HTTP POST request
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(httpResponseCode);
    }

    // Close the HTTP connection
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
