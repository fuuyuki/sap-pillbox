// ---------- 74HC595 pin map ----------
const int PIN_SER   = 23; // DS - Serial Data
const int PIN_CLK   = 18; // SHCP - Shift Clock
const int PIN_LATCH = 19; // STCP - Latch Clock

// ---------- LED animation ----------
enum LedAnim { ANIM_SOLID, ANIM_BLINK };
LedAnim activeAnim = ANIM_BLINK;
const uint32_t BLINK_ON_MS  = 600;
const uint32_t BLINK_OFF_MS = 400;

uint8_t currentMask = 0;
int     activeSlot  = -1;
uint32_t animTimer  = 0;
bool    animPhaseOn = true;

// ---------- Shift register ----------
void shiftSetup() {
  pinMode(PIN_SER,   OUTPUT);
  pinMode(PIN_CLK,   OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  digitalWrite(PIN_SER,   LOW);
  digitalWrite(PIN_CLK,   LOW);
  digitalWrite(PIN_LATCH, LOW);
}

void shiftOutByte(uint8_t data) {
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_SER, (data >> i) & 0x01);
    digitalWrite(PIN_CLK, HIGH);
  }
}

void updateLeds(uint8_t bits) {
  currentMask = bits;
  digitalWrite(PIN_LATCH, LOW);
  shiftOutByte(bits);
  digitalWrite(PIN_LATCH, HIGH);
}

void setSlotSolid(int slot) {
  if (slot < 1 || slot > 7) return;
  activeSlot = slot;
  activeAnim = ANIM_SOLID;
  animPhaseOn = true;
  updateLeds(1 << slot);
}

void setSlotBlink(int slot) {
  if (slot < 1 || slot > 7) return;
  activeSlot = slot;
  activeAnim = ANIM_BLINK;
  animPhaseOn = true;
  animTimer = millis();
  updateLeds(1 << slot);
}

void clearLeds() {
  activeSlot = -1;
  updateLeds(0x00);
}