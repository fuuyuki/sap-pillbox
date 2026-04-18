#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include <Arduino.h>

// Struct for mapping chip IDs to names
struct DeviceMap {
  const char* id;
  const char* name;
};

// Your known devices
static DeviceMap devices[] = {
  {"F4A2B6B24354", "SAP01"},
  {"C87BC4286F24", "SAP02"},
  {"F0B5AD286F24", "SAP03"},
  {"00DE3855B594", "SAP 04"}
};

// Global variables
String chipIdStr;
String deviceName;

// ---------- Helpers ----------
String getChipIdStr() {
  uint64_t chipid = ESP.getEfuseMac();
  char id[13];
  sprintf(id, "%04X%08X",
          (uint16_t)(chipid >> 32),
          (uint32_t)chipid);
  return String(id);
}

String deviceNameFromId(const String& chipId) {
  for (auto &d : devices) {
    if (chipId.equalsIgnoreCase(d.id)) {
      return String(d.name);
    }
  }
  return "Unknown"; // fallback
}

// ---------- Initialization ----------
void initDeviceId() {
  chipIdStr = getChipIdStr();
  deviceName = deviceNameFromId(chipIdStr);

  Serial.printf("Chip ID: %s | Device Name: %s\n",
                chipIdStr.c_str(), deviceName.c_str());
}

#endif