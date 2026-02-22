#include "74hc595.h"
#include "touch.h"

// --- State ---
uint8_t state = 0;  // to change into counter-state
uint8_t ledBits = 0; // reset mask each cycle

// --- Read one sensor ---
bool isTouched(int idx) {
  // int value = touchRead(TOUCH_PINS[idx]);
  int value = digitalRead(TOUCH_PINS[idx]);
  return (value);
}

void updateTouchLeds() {
  ledBits = 0;
  for (int i = 0; i < TOUCH_COUNTS; i++) {
    // Serial.print(i+1); 
    if (isTouched(i)) {
      // Serial.println(" YES");
      state = !state;
      ledBits |= (state << 7-i);   // set bit ON
    }
    // else Serial.println(" NO");
  }
  updateLeds(ledBits); // push to 74HC595
}


// uint16_t baseline[TOUCH_COUNTS];
// bool     touched[TOUCH_COUNTS];
// uint32_t lastChangeMs[TOUCH_COUNTS];

// int16_t readTouchAvg(int pin) {
//   uint32_t sum = 0;
//   for (int i = 0; i < SAMPLE_COUNT; i++) sum += touchRead(pin);
//   return sum / SAMPLE_COUNT;
// }

// void touchCalibrate() {
//   for (int i = 0; i < TOUCH_COUNTS; i++) {
//     uint32_t sum = 0;
//     for (int k = 0; k < BASELINE_SAMPLES; k++) sum += readTouchAvg(TOUCH_PINS[i]);
//     baseline[i] = sum / BASELINE_SAMPLES;
//     touched[i] = false;
//     lastChangeMs[i] = 0;
//   }
// }

// bool touchProximity(int idx) {
//   uint16_t val = readTouchAvg(TOUCH_PINS[idx]);
//   uint16_t onThresh  = baseline[idx] - MARGIN[idx];
//   uint16_t offThresh = baseline[idx] - (MARGIN[idx] - HYSTERESIS[idx]);

//   uint32_t nowMs = millis();
//   if (!touched[idx] && val < onThresh && (nowMs - lastChangeMs[idx] > DEBOUNCE_MS)) {
//     touched[idx] = true;
//     lastChangeMs[idx] = nowMs;
//   } else if (touched[idx] && val > offThresh && (nowMs - lastChangeMs[idx] > DEBOUNCE_MS)) {
//     touched[idx] = false;
//     lastChangeMs[idx] = nowMs;
//   }

//   if (!touched[idx]) baseline[idx] = (baseline[idx] * 31 + val) / 32;
//   return touched[idx];
// }
