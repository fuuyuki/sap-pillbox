#ifndef SHIFT74HC595_H
#define SHIFT74HC595_H

#include <Arduino.h>

// ---------- 74HC595 pin map ----------
const int PIN_SER   = 23; // DS - Serial Data
const int PIN_CLK   = 18; // SHCP - Shift Clock
const int PIN_LATCH = 19; // STCP - Latch Clock

// ---------- State ----------
extern uint8_t ledBits;   // bitmask for LEDs

// ---------- Shift register setup ----------
inline void shiftSetup() {
  pinMode(PIN_SER,   OUTPUT);
  pinMode(PIN_CLK,   OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  digitalWrite(PIN_SER,   LOW);
  digitalWrite(PIN_CLK,   LOW);
  digitalWrite(PIN_LATCH, LOW);
}

// ---------- Shift out one byte ----------
inline void shiftOutByte(uint8_t data) {
  for (int i = 7; i >= 0; --i) {   // MSB first
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_SER, (data >> i) & 0x01);
    digitalWrite(PIN_CLK, HIGH);
  }
}

// ---------- Update LEDs ----------
inline void updateLeds(uint8_t bits) {
  digitalWrite(PIN_LATCH, LOW);
  shiftOutByte(bits);
  digitalWrite(PIN_LATCH, HIGH);
}

// ---------- Clear all LEDs ----------
inline void clearLeds() {
  updateLeds(0x00);
}

#endif
