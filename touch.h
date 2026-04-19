#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>
#include "74hc595.h"

// --- Touch pins (TTP223 digital outputs) ---
// const int TOUCH_PINS[] = {12, 14, 27, 13, 25, 33, 4};
const int TOUCH_PINS[] = {4, 33, 25, 13, 27, 14, 12};
const int TOUCH_COUNTS = sizeof(TOUCH_PINS) / sizeof(TOUCH_PINS[0]);

// --- LED state mask ---
extern uint8_t ledBits;

// --- Setup touch pins ---
inline void touchSetup() {
  for (int i = 0; i < TOUCH_COUNTS; i++) {
    pinMode(TOUCH_PINS[i], INPUT);
  }
}

// --- Read one sensor ---
inline bool isTouched(int idx) {
  return digitalRead(TOUCH_PINS[idx]) == HIGH; // TTP223 outputs HIGH when touched
  // int value = touchRead(TOUCH_PINS[idx]);
  // return (value);
}

// --- Update LEDs based on sensors ---
inline void updateTouchLeds() {
  uint8_t newMask = 0;
  for (int i = 0; i < TOUCH_COUNTS; i++) {
    if (isTouched(i)) {
      newMask |= (1 << i+1);   // LED ON if touched (+1 because we use Q1-Q7)
    }
  }
  // For SAP01
  // for (int i = 0; i < TOUCH_COUNTS; i++) {
  //   if (isTouched(6-i)) {
  //     newMask |= (1 << i+1);   // LED ON if touched (+1 because we use Q1-Q7)
  //   }
  // }
  ledBits = newMask;
  updateLeds(ledBits); // push to 74HC595
}

#endif
