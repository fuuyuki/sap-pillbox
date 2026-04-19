#include <ArduinoOTA.h>
#include "wifi_setup.h"
#include "heartbeat.h"
#include "schedules.h"
#include "touch.h"
#include "alarms.h"
#include "oled.h"
#include "74hc595.h"
#include "device_id.h"

Schedule schedules[10];
int scheduleCount = 0;
uint8_t ledBits = 0;


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

  // oled.h
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // halt
  }

  // 74hc595.h
  shiftSetup();
  clearLeds();
  touchSetup();
  pinMode(PIN_BUZZER,OUTPUT);

  // Initialize chip ID and device name
  initDeviceId();

  // Now you can use chipIdStr and deviceName anywhere
  // For example, show on OLED:
  oledPrint(deviceName.c_str(), "Booting...");
  
  // wifi_setup.h
  wifiSetup();   // handles WiFi + NTP + OLED status

  // OTA setup
  ArduinoOTA.setHostname(deviceName.c_str()); // use SAP_0X name
  ArduinoOTA.onStart([]() {
    Serial.println("Start updating...");
    oledPrint("OTA Update", "Starting...");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char buf[20];
    sprintf(buf, "%u%%", (progress * 100) / total);
    oledPrint("OTA Update", buf);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("Update finished");
    oledPrint("OTA Update", "Done!");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]\n", error);
    oledPrint("OTA Update", "Error!");
  });

  ArduinoOTA.begin();

  Serial.println("OTA ready");
  
  // heartbeat.h
  sendHeartbeat();

  // s
  refreshDeviceData();
  printSchedules();
}

void loop() {
  ArduinoOTA.handle(); // must be called often
  unsigned long currentMillis = millis();

  // Heartbeat interval
  if (currentMillis - lastHeartbeat >= heartbeatInterval) {
    sendHeartbeat();
    lastHeartbeat = currentMillis;
  }

  // Pill check interval
  if (currentMillis - lastCheck >= checkInterval) {
    checkPillTimes();
    lastCheck = currentMillis;
  }

  // OLED time update interval
  if (currentMillis - lastOledUpdate >= oledInterval) {
    checkPillTimes();
    struct tm now;
    
    if (getLocalTime(&now)) {
      bool online = (WiFi.status() == WL_CONNECTED);
      oledShowTime(now, online, heartbeatActive, deviceName.c_str());
      // oledShowTime(now, "Online"); // status can be "Online", "Offline", etc.
    }
    lastOledUpdate = currentMillis; 
  }

  // Refresh schedules
  if (currentMillis - lastScheduleRefresh >= scheduleRefreshInterval) {
    refreshDeviceData();
    printSchedules(); // optional debug
    lastScheduleRefresh = currentMillis; 
  }

  // Debug test LEDs
  // ledTest();
  // oledPrint("LED Testing", "Running...");
  
  // Debug test touchs
  // updateTouchLeds();
  // // Debug print
  // oledPrint("Touch Test", "Running...");
  // Serial.print("Touched mask: ");
  // for (int i = TOUCH_COUNTS - 1; i >= 0; i--) {
  //   Serial.print((ledBits & (1 << i)) ? "1" : "0");
  // }
  // Serial.println();

  // delay(100);
}
