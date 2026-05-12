/*
  Dashboard RC Racing Style
  Board  : Waveshare ESP32-C6-LCD-1.47"
  LCD    : ST7789 172x320
  Library:
    - GFX Library for Arduino
    - Adafruit GFX Library
*/

#include <Arduino_GFX_Library.h>
#include <Adafruit_GFX.h>

// ===== Warna =====
#define BLACK     0x0000
#define WHITE     0xFFFF
#define RED       0xF800
#define GREEN     0x07E0
#define BLUE      0x001F
#define CYAN      0x07FF
#define YELLOW    0xFFE0
#define DARKGREY  0x7BEF

// ===== Pin LCD =====
#define TFT_BL    15
#define TFT_DC     4
#define TFT_CS     5
#define TFT_SCLK   6
#define TFT_MOSI   7
#define TFT_RST    8

// ===== SPI Bus =====
Arduino_DataBus *bus = new Arduino_ESP32SPI(
  TFT_DC,
  TFT_CS,
  TFT_SCLK,
  TFT_MOSI,
  GFX_NOT_DEFINED
);

// ===== LCD ST7789 =====
Arduino_GFX *gfx = new Arduino_ST7789(
  bus,
  TFT_RST,
  1,
  true,
  172,
  320,
  34,
  0,
  34,
  0
);

// ===== Data Dashboard =====
int speedVal = 0;
int batteryVal = 100;
int rssiVal = 100;
int gear = 0;

// ===== RPM Bar =====
void drawRPMBar(int rpm)
{
  int bars = map(rpm, 0, 120, 0, 20);

  for (int i = 0; i < 20; i++) {
    uint16_t color = GREEN;
    if (i > 12) color = YELLOW;
    if (i > 16) color = RED;

    if (i < bars) {
      gfx->fillRect(10 + i * 15, 10, 14, 10, color);
    } else {
      gfx->drawRect(10 + i * 15, 10, 14, 10, DARKGREY);
    }
  }
}

// ===== Render Dashboard =====
void drawDashboard()
{
  gfx->fillScreen(BLACK);

  drawRPMBar(speedVal);

  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  gfx->setCursor(220, 70);
  gfx->print("km/h");

  gfx->setTextColor(CYAN);
  gfx->setTextSize(6);
  gfx->setCursor(60, 90);
  gfx->printf("%03d", speedVal);

  const char *gears[] = {"N", "1", "2", "3"};
  gfx->setTextColor(YELLOW);
  gfx->setTextSize(5);
  gfx->setCursor(10, 90);
  gfx->print(gears[gear]);

  gfx->setTextColor(GREEN);
  gfx->setTextSize(2);
  gfx->setCursor(10, 220);
  gfx->printf("BAT: %d%%", batteryVal);

  gfx->setTextColor(WHITE);
  gfx->setCursor(10, 250);
  gfx->printf("RSSI: %d%%", rssiVal);

  gfx->setTextColor(DARKGREY);
  gfx->setCursor(10, 300);
  gfx->print("ESP32-C6 RC DASH");
}

// ===== Setup =====
void setup()
{
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  Serial.begin(115200);

  gfx->begin();
  gfx->setRotation(1);

  drawDashboard();
}

// ===== Loop =====
void loop()
{
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > 200) {
    lastUpdate = millis();

    speedVal++;
    if (speedVal > 120) speed