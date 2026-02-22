#ifndef ALARMS_H
#define ALARMS_H

#include "time.h"
#include "schedules.h"
#include "74hc595.h"
#include <HTTPClient.h>

// Declare extern so they’re defined in main.ino
extern String apiKey; 
extern String chipIdStr;  // device_id
const unsigned long alarmTimeoutMs = 30UL * 1000UL; // 30 seconds
// const unsigned long alarmTimeoutMs = 3UL * 60UL * 60UL * 1000UL // 3 hours
const int PIN_BUZZER = 26;

String userId = ""; // will be fetched

void fetchUserId() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://sap.protofylabs.web.id/devices?chip_id=" + chipIdStr;
    http.begin(url);
    http.addHeader("x-api-key", apiKey);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<512> doc;
      if (deserializeJson(doc, payload) == DeserializationError::Ok) {
        userId = doc["user_id"].as<String>();
        Serial.printf("Fetched user_id: %s\n", userId.c_str());
      }
    }
    http.end();
  }
}

// Encapsulated notification sender
void sendNotification(const String &pillname, const struct tm &now) {
  // Format created_at timestamp
  char timeStr[40];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%S", &now);
  String createdAt = String(timeStr) + ".000Z";

  // Build JSON payload
  String message = "Time to take " + pillname;
  String payload = "{\"message\":\"" + message +
                   "\",\"device_id\":\"" + chipIdStr +
                   "\",\"user_id\":\"" + userId +
                   "\",\"created_at\":\"" + createdAt + "\"}";

  // Send POST /notifications/{chip_id}
  HTTPClient http;
  String url = "https://sap.protofylabs.web.id/notifications/" + chipIdStr;
  http.begin(url);
  http.addHeader("x-api-key", apiKey);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(payload);
  Serial.printf("Notification response: %d\n", httpResponseCode);
  http.end();
}

void sendMedlog(String pillname, struct tm now, String status) {
  char timeStr[40];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%S", &now);
  // String scheduledTime = String(timeStr) + ".000Z";
  String scheduledTime = String(timeStr);

  String payload = "{\"pillname\":\"" + pillname +
                 "\",\"scheduled_time\":\"" + scheduledTime +
                 "\",\"status\":\"" + status +
                 "\",\"user_id\":\"" + userId +
                 "\",\"device_id\":\"" + chipIdStr + "\"}";

  HTTPClient http;
  String url = "https://sap.protofylabs.web.id/medlogs/" + chipIdStr;
  http.begin(url);
  http.addHeader("x-api-key", apiKey);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(payload);
  Serial.printf("Medlog (%s) response: %d\n", status.c_str(), httpResponseCode);
  http.end();
}


void checkPillTimes() {
  struct tm now;
  if (!getLocalTime(&now)) return;

  // Declare today's day index here
  int todayIdx = now.tm_wday; // 0 = Sunday, 6 = Saturday

  for (int i = 0; i < scheduleCount; i++) {
    // Activation: exact time match
    if (now.tm_hour == schedules[i].doseTm.tm_hour &&
        now.tm_min  == schedules[i].doseTm.tm_min &&
        schedules[i].lastNotifiedDay != now.tm_mday) {
        
      Serial.printf(">>> Alarm ON for %s! <<<\n", schedules[i].pillname.c_str());

      // Update LEDs: turn on today's slot LED
      digitalWrite(PIN_BUZZER,HIGH);
      ledBits |= (1 << 7-todayIdx); updateLeds(ledBits);

      schedules[i].alarmActive = true;
      schedules[i].alarmStart = millis();         // start counting
      schedules[i].lastNotifiedDay = now.tm_mday; // Mark as notified today

      sendNotification(schedules[i].pillname, now);
    }

    // Deactivation: touch sensor pressed
    if (schedules[i].alarmActive && isTouched(todayIdx)){
      Serial.printf(">>> Alarm OFF, pill taken: %s <<<\n", schedules[i].pillname.c_str());
      digitalWrite(PIN_BUZZER, LOW);
      schedules[i].alarmActive = false;

      // Update LEDs: clear today's slot LED
      ledBits &= ~(1 << 7-todayIdx); updateLeds(ledBits);

      // Send medlog with status "taken"
      sendMedlog(schedules[i].pillname, now, "taken");
    }

    // Timeout: 3 hours passed without touch
    // 3UL = 3 (unsigned long)
    // 60UL * 60UL = 3600 seconds
    // * 1000UL = convert seconds to milliseconds → 10,800,000 ms
    if (schedules[i].alarmActive &&
        millis() - schedules[i].alarmStart > alarmTimeoutMs){
      Serial.printf(">>> Alarm timeout, pill missed: %s <<<\n", schedules[i].pillname.c_str());
      digitalWrite(PIN_BUZZER, LOW);
      schedules[i].alarmActive = false;

      // Update LEDs: clear today's slot LED
      ledBits &= ~(1 << 7-todayIdx); updateLeds(ledBits);

      // Send medlog with status "missed"
      sendMedlog(schedules[i].pillname, now, "missed");
    }
  }
}


// Combined refresh
void refreshDeviceData() { 
  fetchUserId(); 
  fetchSchedules();
}

#endif
