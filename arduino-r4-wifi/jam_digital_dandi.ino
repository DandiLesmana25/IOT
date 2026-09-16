/**
 * RTC_NTPSync
 * 
 * This example shows how to set the RTC (Real Time Clock) on the Portenta C33 / UNO R4 WiFi
 * to the current date and time retrieved from an NTP server on the Internet (pool.ntp.org).
 * Then the current time from the RTC is printed to the Serial port.
 * 
 * Instructions:
 * 1. Download the NTPClient library (https://github.com/arduino-libraries/NTPClient) through the Library Manager
 * 2. Change the WiFi credentials in the arduino_secrets.h file to match your WiFi network.
 * 3. Upload this sketch to Portenta C33 / UNO R4 WiFi.
 * 4. Open the Serial Monitor.
 * 
 * Initial author: Sebastian Romero @sebromero
 * 
 * Find the full UNO R4 WiFi RTC documentation here:
 * https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc
 */

// Include the RTC library
#include "RTC.h"

//Include the NTP library
#include <NTPClient.h>

#if defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#endif

#include <WiFiUdp.h>

// ---  LIBRARY 7 segment TM1637 ---
#include <TM1637Display.h>



const char ssid[] = "nama_wifi";
const char pass[] = "pw_wifi";

int wifiStatus = WL_IDLE_STATUS;
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

// --- KONFIGURASI TM1637 ---
#define CLK 3
#define DIO 2
TM1637Display display(CLK, DIO);
bool titikDuaNyala = false; // Toggle untuk animasi detik berkedip


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi(){
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to WiFi");
  printWifiStatus();
}

void setup(){
  Serial.begin(9600);
  while (!Serial);

  // Nyalakan Layar 7-Segment terlebih dahulu
  display.setBrightness(0x0f); 
  display.clear();

  connectToWiFi();
  RTC.begin();

  Serial.println("\nStarting connection to server...");
  timeClient.begin();
  timeClient.update();

  // Get the current date and time from an NTP server and convert
  // it to UTC +2 by passing the time zone offset in hours.
  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = 7;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print("Unix time = ");
  Serial.println(unixTime);

  // Setel mesin jam RTC internal R4
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);

  Serial.println("The RTC was successfully synced!");

  // Layar dibersihkan sebelum masuk ke loop
  display.clear();
}

void loop(){
  // 1. Ambil Waktu dari RTC (Offline)
  RTCTime currentTime;
  RTC.getTime(currentTime); 
  
  int jam = currentTime.getHour();
  int menit = currentTime.getMinutes();

  // 2. Format Waktu untuk Layar TM1637
  // Gabungkan jam dan menit jadi satu angka (misal: 14 dan 30 jadi 1430)
  int waktuGabungan = (jam * 100) + menit;
  
  // 3. Animasi Titik Dua (Berkedip per detik)
  titikDuaNyala = !titikDuaNyala;
  
  // 0b01000000 adalah kode biner standar untuk menyalakan titik dua di tengah.
  // Catatan: Efek ini sangat bergantung pada pabrikan fisik modul TM1637.
  if (titikDuaNyala) {
    display.showNumberDecEx(waktuGabungan, 0b01000000, true); 
  } else {
    display.showNumberDecEx(waktuGabungan, 0b00000000, true);
  }

  // print waktu untuk peperluan debugging awal
  // Serial.println("Waktu RTC: " + String(currentTime));

  // Jeda 1 detik (1000ms) untuk mengulang pembacaan dan kedipan
  delay(1000);
}
