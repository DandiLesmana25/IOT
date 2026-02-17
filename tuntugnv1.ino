#include <Arduino.h>
#include <LittleFS.h>
#include "AudioFileSourceLittleFS.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// --- Pin Definitions for ESP32-S3 ---
// You can change these to any valid GPIOs on the S3
#define I2S_LRC     17 
#define I2S_BCLK    16
#define I2S_DIN     18

// --- Audio Objects ---
AudioGeneratorMP3 *mp3;
AudioFileSourceLittleFS *file;
AudioOutputI2S *out;



// ... includes remain the same ...

void setup() {
  Serial.begin(115200);
  delay(2000); // Give serial time to wake up

  // 1. Mount LittleFS
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  Serial.println("LittleFS Mounted");

  // 2. CHECK FILE EXISTENCE & SIZE (New Debug Step)
  if (LittleFS.exists("/alert.mp3")) {
      File f = LittleFS.open("/alert.mp3", "r");
      Serial.print("File found! Size: ");
      Serial.println(f.size());
      f.close();
      if (f.size() == 0) {
          Serial.println("ERROR: File is empty (0 bytes). Re-upload filesystem.");
          return;
      }
  } else {
      Serial.println("ERROR: /alert.mp3 does not exist!");
      return;
  }

  // 3. Setup I2S Output (EXPLICIT MODE)
  // Use (0, 1) -> Port 0, External I2S Mode (Correct for MAX98357A)
  out = new AudioOutputI2S(0, 1); 
  
  // Pin Mapping: BCLK, LRC, DIN
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DIN);
  out->SetGain(1.0);

  // 4. Initialize MP3
  file = new AudioFileSourceLittleFS("/alert.mp3");
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  
  Serial.println("Playback started...");
}



void loop() {
  // 5. Keep the buffer filled
   Serial.println(mp3->isRunning());
  if (mp3->isRunning()) {
    if (!mp3->loop()) { 
      // If loop() returns false, the song is over
      mp3->stop();
      Serial.println("Playback finished");
    }
  } else {
    // Optional: Add logic here to replay or play a different file
    Serial.println("Loop Running - Audio Finished");
    delay(1000);
  }
}
