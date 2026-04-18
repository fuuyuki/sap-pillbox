#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "time.h"

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SDA_PIN 16
#define SCL_PIN 17

// ✔ Check icon
const unsigned char checkIcon[] PROGMEM = {
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000110,
  0b11001100,
  0b11111000,
  0b01110000,
  0b00000000
};

// ✖ Cross icon
const unsigned char crossIcon[] PROGMEM = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};

// ♥ Heart icon
const unsigned char heartIcon[] PROGMEM = {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00000000
};


// ---------- OLED helpers ----------
void oledPrint(const char* line1, const char* line2 = "") {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(line1);
  if (strlen(line2) > 0) {
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println(line2);
  }
  display.display();
}

// Draw device name in bottom-right corner
void oledShowDeviceName(const char* name) {
  int16_t x, y;
  uint16_t w, h;

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Measure text width/height
  display.getTextBounds(name, 0, 0, &x, &y, &w, &h);

  // Position bottom-right
  int xpos = SCREEN_WIDTH - w - 2;  // 2px padding
  int ypos = SCREEN_HEIGHT - h - 2;

  display.setCursor(xpos, ypos);
  display.print(name);
}

void oledShowTime(const struct tm &t, bool online,  bool heartbeat, const char* deviceName) {
  display.clearDisplay();

  // Show time
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.printf("%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);

  // Show icons on second line
  int y = 20;
  if (online) {
    display.setCursor(0, y);
    display.setTextSize(1);
    display.print("WiFi");
    display.drawBitmap(25, y, checkIcon, 8, 8, SSD1306_WHITE);
  } else {
    display.setCursor(0, y);
    display.setTextSize(1);
    display.print("No WiFi");
    display.drawBitmap(25, y, crossIcon, 8, 8, SSD1306_WHITE);
  }

  if (heartbeat) {
    display.setCursor(40, y);
    display.setTextSize(1);
    display.print("Server");
    display.drawBitmap(80, y, heartIcon, 8, 8, SSD1306_WHITE);
  } else {
    display.setCursor(40, y);
    display.setTextSize(1);
    display.print("Server");
    display.drawBitmap(80, y, crossIcon, 8, 8, SSD1306_WHITE);
  }

  // Always show device name at bottom-right
  oledShowDeviceName(deviceName);

  display.display();
}

#endif
