#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFiManager.h>
#include "oled.h"
#include "device_id.h"
#include <time.h>

// NTP server settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;   // Jakarta GMT+7
const int daylightOffset_sec = 0;

// ---------- WiFi + NTP Setup ----------
void wifiSetup() {

  WiFiManager wifiManager;
  wifiManager.setTimeout(180); // 3 minutes fallback

  oledPrint("WiFi Setup", "Connecting...");

  // Try saved credentials, else start AP
  // Use deviceName (SAP_0X) as the AP SSID
  if (!wifiManager.autoConnect(deviceName.c_str())) {
    oledPrint("WiFi Failed", "Restarting...");
    ESP.restart();
  }

  oledPrint("WiFi OK", WiFi.SSID().c_str());
  Serial.println("Connected to WiFi!");

  // Configure NTP
  oledPrint("NTP Sync", "Getting time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    oledPrint("NTP Failed", "Restarting...");
    delay(2000);
    ESP.restart();
  }
}

#endif
