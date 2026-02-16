#include "touch.h"

uint16_t baseline[TOUCH_COUNTS];
bool     touched[TOUCH_COUNTS];
uint32_t lastChangeMs[TOUCH_COUNTS];

int16_t readTouchAvg(int pin) {
  uint32_t sum = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) sum += touchRead(pin);
  return sum / SAMPLE_COUNT;
}

void touchCalibrate() {
  for (int i = 0; i < TOUCH_COUNTS; i++) {
    uint32_t sum = 0;
    for (int k = 0; k < BASELINE_SAMPLES; k++) sum += readTouchAvg(TOUCH_PINS[i]);
    baseline[i] = sum / BASELINE_SAMPLES;
    touched[i] = false;
    lastChangeMs[i] = 0;
  }
}

bool touchProximity(int idx) {
  uint16_t val = readTouchAvg(TOUCH_PINS[idx]);
  uint16_t onThresh  = baseline[idx] - MARGIN[idx];
  uint16_t offThresh = baseline[idx] - (MARGIN[idx] - HYSTERESIS[idx]);

  uint32_t nowMs = millis();
  if (!touched[idx] && val < onThresh && (nowMs - lastChangeMs[idx] > DEBOUNCE_MS)) {
    touched[idx] = true;
    lastChangeMs[idx] = nowMs;
  } else if (touched[idx] && val > offThresh && (nowMs - lastChangeMs[idx] > DEBOUNCE_MS)) {
    touched[idx] = false;
    lastChangeMs[idx] = nowMs;
  }

  if (!touched[idx]) baseline[idx] = (baseline[idx] * 31 + val) / 32;
  return touched[idx];
}
