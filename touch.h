#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>

const int TOUCH_PINS[]       = {12, 14, 27, 13, 15, 2, 4};
const int TOUCH_COUNTS       = sizeof(TOUCH_PINS)/sizeof(TOUCH_PINS[0]);
const int SAMPLE_COUNT       = 8;
const int BASELINE_SAMPLES   = 360;
const int MARGIN[7]          = {60, 80, 60, 50, 45, 50, 55};
const int HYSTERESIS[7]      = {40, 50, 40, 35, 30, 35, 40};
const int DEBOUNCE_MS        = 50;

// Declare globals (not define!)
extern uint16_t baseline[TOUCH_COUNTS];
extern bool     touched[TOUCH_COUNTS];
extern uint32_t lastChangeMs[TOUCH_COUNTS];

int16_t readTouchAvg(int pin);
void touchCalibrate();
bool touchProximity(int idx);

#endif
