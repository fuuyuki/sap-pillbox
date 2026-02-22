#include "wifi_setup.h"
#include "heartbeat.h"
#include "schedules.h"
#include "touch.h"
#include "alarms.h"
#include "oled.h"
#include "74hc595.h"

String apiKey = "b6d1c1cda37e9e8bd4d3730d8a20f51882c5f97075b2cdc18c683f320a3172bb";
String chipIdStr = "C87BC4286F24";

Schedule schedules[100];
int scheduleCount = 0;

unsigned long lastHeartbeat = 0;
const unsigned long heartbeatInterval = 30000; // 30s

unsigned long lastCheck = 0;
const unsigned long checkInterval = 60000; // 60s

unsigned long lastOledUpdate = 0;
const unsigned long oledInterval = 1000; // update OLED every second

unsigned long lastScheduleRefresh = 0;
const unsigned long scheduleRefreshInterval = 900000; // 15 minutes


void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // halt
  } 
  display.clearDisplay();
  display.display();

  shiftSetup();
  clearLeds();
  pinMode(PIN_BUZZER,OUTPUT);

  connectWiFi();
  sendHeartbeat();

  refreshDeviceData();
  printSchedules();
}

void loop() {
  unsigned long currentMillis = millis();

  // Heartbeat interval
  if (currentMillis - lastHeartbeat >= heartbeatInterval) {
    sendHeartbeat();
    lastHeartbeat = currentMillis;
  }

  // Pill check interval
  // if (currentMillis - lastCheck >= checkInterval) {
  //   checkPillTimes();
  //   lastCheck = currentMillis;
  // }

  // OLED time update interval
  if (currentMillis - lastOledUpdate >= oledInterval) {
    checkPillTimes();
    struct tm now;
    if (getLocalTime(&now)) {
      oledShowTime(now, "Online"); // status can be "Online", "Offline", etc.
    }
    lastOledUpdate = currentMillis; 
  }

  // Refresh schedules
  if (currentMillis - lastScheduleRefresh >= scheduleRefreshInterval) {
    refreshDeviceData();
    printSchedules(); // optional debug
    lastScheduleRefresh = currentMillis; 
  }

  // Debug test touchs + LEDs
  // updateTouchLeds();
  // delay(100);

}
