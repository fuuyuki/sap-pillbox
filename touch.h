#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>

// --- Touch pins ---
const int TOUCH_PINS[] = {4, 2, 15, 13, 27, 14, 12};
const int TOUCH_COUNTS = 7;

// // --- Thresholds per sensor (adjust individually) ---
// int TOUCH_THRESHOLDS[TOUCH_COUNTS] = {
//   30, // sensor 0 (pin 12)
//   30, // sensor 1 (pin 14)
//   50, // sensor 2 (pin 27)
//   30, // sensor 3 (pin 13)
//   50, // sensor 4 (pin 15) <-- higher sensitivity for hard-to-recognize sensor
//   30, // sensor 5 (pin 2)
//   50 // sensor 6 (pin 4)
// };

bool isTouched(int idx);
void updateTouchLeds();

// const int TOUCH_PINS[]       = {12, 14, 27, 13, 15, 2, 4};
// const int TOUCH_COUNTS       = sizeof(TOUCH_PINS)/sizeof(TOUCH_PINS[0]);
// const int SAMPLE_COUNT       = 8;
// const int BASELINE_SAMPLES   = 360;
// const int MARGIN[7]          = {60, 80, 60, 50, 45, 50, 55};
// const int HYSTERESIS[7]      = {40, 50, 40, 35, 30, 35, 40};
// const int DEBOUNCE_MS        = 50;

// // Declare globals (not define!)
// extern uint16_t baseline[TOUCH_COUNTS];
// extern bool     touched[TOUCH_COUNTS];
// extern uint32_t lastChangeMs[TOUCH_COUNTS];

// int16_t readTouchAvg(int pin);
// void touchCalibrate();
// bool touchProximity(int idx);

#endif
