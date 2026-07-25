| Pin di Modul RC522 | Dihubungkan ke Pin Arduino R4 WiFi | Keterangan / Fungsi |
| :--- | :--- | :--- |
| **SDA / SS** | **Pin 10** | Slave Select (Diatur di kode: `#define SS_PIN 10`) |
| **SCK** | **Pin 13** | *Serial Clock (Jalur clock standar SPI Arduino)* |
| **MOSI** | **Pin 11** | *Master Out Slave In (Jalur kirim data standar SPI)* |
| **MISO** | **Pin 12** | *Master In Slave Out (Jalur terima data standar SPI)* |
| **IRQ** | **Tidak Terhubung** | *Interrupt Request (Tidak digunakan pada project ini)* |
| **GND** | **GND** | *Ground (Kutub Negatif)* |
| **RST** | **Pin 9** | Reset (Diatur di kode: `#define RST_PIN 9`) |
| **3.3V** | **3.3V** | Daya Modul. **(JANGAN HUBUNGKAN KE 5V!)** |