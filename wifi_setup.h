#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

const char* ssid = "Mangku Fuhrer";
const char* password = "terkoneksi";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;   // GMT+7 for Western Indonesia Time
const int daylightOffset_sec = 0;

void connectWiFi() {
  pinMode(22,OUTPUT);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(22,HIGH);
    delay(500);
    Serial.print(".");
  }
  digitalWrite(22,LOW);
  // Serial.println("\nConnected to WiFi!");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

#endif
