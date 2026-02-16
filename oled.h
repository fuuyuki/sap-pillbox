#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SDA_PIN 16
#define SCL_PIN 17

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


void oledShowTime(const struct tm &t, const char* status = "") {
  // Print to Serial for debugging
  Serial.printf("Current Time: %02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  if (strlen(status) > 0) {
    Serial.printf(" | Status: %s", status);
  }
  Serial.println();

  // Show on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.printf("%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  if (strlen(status) > 0) {
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println(status);
  }
  display.display();
}

#endif
