/*
  Dashboard RC Racing Style
  Board  : Waveshare ESP32-C6-LCD-1.47"
  LCD    : ST7789 172x320
  Library: Arduino_GFX Library
*/

#include <Arduino_GFX_Library.h>

// ===== Definisi Warna =====
#define BLACK     0x0000
#define WHITE     0xFFFF
#define RED       0xF800
#define GREEN     0x07E0
#define BLUE      0x001F
#define CYAN      0x07FF
#define YELLOW    0xFFE0
#define DARKGREY  0x7BEF

// ===== Pin LCD Waveshare ESP32-C6-LCD-1.47" =====
#define TFT_BL    15
#define TFT_DC     4
#define TFT_CS     5
#define TFT_SCLK   6
#define TFT_MOSI   7
#define TFT_RST    8

// SPI Bus
Arduino_DataBus *bus = new Arduino_ESP32SPI(
  TFT_DC,      // DC
  TFT_CS,      // CS
  TFT_SCLK,    // SCK
  TFT_MOSI,    // MOSI
  -1           // MISO not used
);

// ST7789 LCD 172x320
Arduino_GFX *gfx = new Arduino_ST7789(
  bus,
  TFT_RST,
  0,           // rotation
  true,        // IPS
  172,         // width
  320,         // height
  34,          // col offset 1
  0,           // row offset 1
  34,          // col offset 2
  0            // row offset 2
);

// ===== Variabel Dashboard =====
int speedVal = 0;       // km/h
int batteryVal = 100;   // %
int rssiVal = 100;      // %
int gear = 0;           // 0=N, 1=1, 2=2, 3=3

// ===== Gambar Bar RPM =====
void drawRPMBar(int rpm)
{
  int bars = map(rpm, 0, 120, 0, 20);
  int x = 10;
  int y = 10;
  int w = 14;
  int h = 10;

  for (int i = 0; i < 20; i++) {
    uint16_t color = GREEN;
    if (i > 12) color = YELLOW;
    if (i > 16) color = RED;

    if (i < bars) {
      gfx->fillRect(x + i * (w + 1), y, w, h, color);
    } else {
      gfx->drawRect(x + i * (w + 1), y, w, h, DARKGREY);
    }
  }
}

// ===== Gambar Dashboard =====
void drawDashboard()
{
  gfx->fillScreen(BLACK);

  // RPM bar
  drawRPMBar(speedVal);

  // Label km/h
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  gfx->setCursor(220, 70);
  gfx->print("km/h");

  // Speed besar
  gfx->setTextColor(CYAN);
  gfx->setTextSize(6);
  gfx->setCursor(60, 90);
  gfx->printf("%03d", speedVal);

  // Gear
  const char *gears[] = {"N", "1", "2", "3"};
  gfx->setTextColor(YELLOW);
  gfx->setTextSize(5);
  gfx->setCursor(10, 90);
  gfx->print(gears[gear]);

  // Battery
  gfx->setTextColor(GREEN);
  gfx->setTextSize(2);
  gfx->setCursor(10, 220);
  gfx->printf("BAT: %d%%", batteryVal);

  // RSSI
  gfx->setTextColor(WHITE);
  gfx->setCursor(10, 250);
  gfx->printf("RSSI: %d%%", rssiVal);

  // Footer
  gfx->setTextColor(DARKGREY);
  gfx->setCursor(10, 300);
  gfx->print("Waveshare ESP32-C6 RC Dashboard");
}

// ===== Setup =====
void setup()
{
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // nyalakan backlight

  gfx->begin();
  gfx->setRotation(1); // landscape

  drawDashboard();
}

// ===== Loop =====
void loop()
{
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > 200) {
    lastUpdate = millis();

    // Simulasi speed
    speedVal++;
    if (speedVal > 120) speedVal = 0;

    // Simulasi battery
    batteryVal = 100 - speedVal / 2;
    if (batteryVal < 0) batteryVal = 100;

    // Simulasi RSSI
    rssiVal = 100 - (speedVal % 40);

    // Simulasi gear
    gear = (speedVal / 30) % 4;

    drawDashboard();
  }
}