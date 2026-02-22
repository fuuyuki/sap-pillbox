#ifndef SCHEDULES_H
#define SCHEDULES_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"

struct Schedule {
  String pillname;
  struct tm doseTm;
  int repeat_days;
  int lastNotifiedDay;      // default -1
  bool alarmActive;         // is buzzer currently on
  unsigned long alarmStart; // millis when alarm started
};

extern String apiKey;
extern String chipIdStr;
extern Schedule schedules[100];   // adjust size as needed
extern int scheduleCount;

bool parseDoseTime(const char* timeStr, struct tm* tmOut) {
  int hour, minute, second;
  // Try parsing time-only format
  if (sscanf(timeStr, "%d:%d:%d", &hour, &minute, &second) == 3) {
    // Fill tmOut with current date + provided time
    struct tm now;
    if (getLocalTime(&now)) {
      tmOut->tm_year = now.tm_year;
      tmOut->tm_mon  = now.tm_mon;
      tmOut->tm_mday = now.tm_mday;
    } else {
      // fallback: set to Jan 1, 2000
      tmOut->tm_year = 100; // 2000
      tmOut->tm_mon  = 0;
      tmOut->tm_mday = 1;
    }
    tmOut->tm_hour = hour;
    tmOut->tm_min  = minute;
    tmOut->tm_sec  = second;
    return true;
  }

  // // Otherwise try full ISO8601 format
  // int year, month, day;
  // if (sscanf(timeStr, "%d-%d-%dT%d:%d:%d",
  //            &year, &month, &day, &hour, &minute, &second) == 6) {
  //   tmOut->tm_year = year - 1900;
  //   tmOut->tm_mon  = month - 1;
  //   tmOut->tm_mday = day;
  //   tmOut->tm_hour = hour;
  //   tmOut->tm_min  = minute;
  //   tmOut->tm_sec  = second;
  //   return true;
  // }

  return false; // failed to parse
}


// Fetch schedules and store them
void fetchSchedules() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://sap.protofylabs.web.id/schedules/device/" + chipIdStr;
    http.begin(url);
    http.addHeader("x-api-key", apiKey);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<2048> doc;
      if (deserializeJson(doc, payload) == DeserializationError::Ok) {
        scheduleCount = 0;
        for (JsonObject item : doc.as<JsonArray>()) {
          if (scheduleCount >= 50) break; // prevent overflow
          schedules[scheduleCount].pillname = item["pillname"].as<String>();
          parseDoseTime(item["dose_time"], &schedules[scheduleCount].doseTm);
          schedules[scheduleCount].repeat_days = item["repeat_days"];
          scheduleCount++;
        }
      }
    }
    http.end();
  }
}

// Print schedules stored in memory
void printSchedules() {
  Serial.println("=== Stored Schedules ===");
  for (int i = 0; i < scheduleCount; i++) {
    Schedule s = schedules[i];
    Serial.printf("[%d] Pill: %s\n", i, s.pillname.c_str());
    Serial.printf("    Time: %02d:%02d\n", s.doseTm.tm_hour, s.doseTm.tm_min);
    Serial.printf("    Repeat Days: %d\n", s.repeat_days);
    Serial.println();
  }
}

#endif
