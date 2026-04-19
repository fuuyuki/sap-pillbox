#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include <Arduino.h>

// Struct for mapping chip IDs to names
struct DeviceMap {
  const char* id;
  const char* name;
  const char* key;
};

// Your known devices
static DeviceMap devices[] = {
  {"F4A2B6B24354", "SAP01", "7cc30933c7b4ad980886fda2e58f1e01539d30620449bd05e742c6c1d2fffd04"},
  {"C87BC4286F24", "SAP02", "b6d1c1cda37e9e8bd4d3730d8a20f51882c5f97075b2cdc18c683f320a3172bb"},
  {"F0B5AD286F24", "SAP03", "08f5d1f653b0b65ef3b4600e74890fcc06b1130733023142e0658ee9a338bb8c"},
  {"00DE3855B594", "SAP04", "71d3880b72db1e18f23cec342897fb155b25de1876990f08e6e1c173a53ef754"}
};

// Global variables
String chipIdStr;
String deviceName;
String apiKey;

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

String apiKeyFromId(const String& chipId) {
  for (auto &d : devices) {
    if (chipId.equalsIgnoreCase(d.id)) {
      return String(d.key);
    }
  }
  return "Unknown"; // fallback
}

// ---------- Initialization ----------
void initDeviceId() {
  chipIdStr = getChipIdStr();
  apiKey = apiKeyFromId(chipIdStr);
  deviceName = deviceNameFromId(chipIdStr);

  Serial.printf("Chip ID: %s | Device Name: %s\n",
                chipIdStr.c_str(), deviceName.c_str());
}

#endif