#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <HTTPClient.h>
#include "time.h"

// Declare extern so they are defined in main.ino
extern String apiKey;
extern String chipIdStr;

void sendHeartbeat() {
  if (WiFi.status() == WL_CONNECTED) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }

    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%S%z", &timeinfo);
    String payload = "{\"last_seen\":\"" + String(timeStr) + "\"}";

    HTTPClient http;
    String url = "https://sap.protofylabs.web.id/devices/" + chipIdStr + "/heartbeat/";
    http.begin(url);
    http.addHeader("x-api-key", apiKey);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);
    Serial.printf("Heartbeat response: %d\n", httpResponseCode);
    http.end();
  }
}

#endif
