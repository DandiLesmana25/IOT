#include <driver/i2s.h>  // Library built-in ESP32
#include <math.h>

#define I2S_PORT I2S_NUM_0  // I2S port 0
#define PIN_DIN 6           // GPIO6 untuk DIN
#define PIN_LRC 5           // GPIO5 untuk LRC
#define PIN_BCLK 4          // GPIO4 untuk BCLK
#define SAMPLE_RATE 44100   // Sample rate standar audio
#define TONE_FREQ 500       // Frekuensi tone (Hz)
#define AMPLITUDE 20000      // Amplitudo (volume)
#define BUFFER_LEN 256      // Ukuran buffer

int16_t samples[BUFFER_LEN * 2];  // Buffer stereo

void setup() {
  Serial.begin(115200);

  // Konfigurasi I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = BUFFER_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  // Set pin I2S
  i2s_pin_config_t pin_config = {
    .bck_io_num = PIN_BCLK,
    .ws_io_num = PIN_LRC,
    .data_out_num = PIN_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  // Instal driver dan set pin
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

void loop() {
  static float phase = 0.0;
  const float phaseIncrement = 2.0 * PI * TONE_FREQ / SAMPLE_RATE;

  // Generate samples
  for (int i = 0; i < BUFFER_LEN; i++) {
    int16_t sound = (int16_t)(AMPLITUDE * sin(phase));
    samples[2 * i] = sound;     // Left channel
    samples[2 * i + 1] = sound; // Right channel (mono)
    phase += phaseIncrement;
    if (phase >= 2.0 * PI) phase -= 2.0 * PI;
  }

  // Kirim ke I2S
  size_t bytes_written;
  i2s_write(I2S_PORT, samples, sizeof(samples), &bytes_written, portMAX_DELAY);
}
