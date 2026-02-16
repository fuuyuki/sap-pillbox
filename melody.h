// ---------- Buzzer ----------
void playBootupMelody() {
  int notes[] = {262, 330, 392, 523};
  int durations[] = {200, 200, 200, 400};
  for (int i = 0; i < 4; i++) {
    tone(PIN_BUZZER, notes[i], durations[i]);
    delay(durations[i] + 50);
  }
}

void playMedicineStandbyMelody() {
  int notes[] = {784, 659};
  int durations[] = {300, 300};
  for (int i = 0; i < 2; i++) {
    tone(PIN_BUZZER, notes[i], durations[i]);
    delay(durations[i] + 100);
  }
}

void playMedicineTakenMelody() {
  int notes[] = {523, 392, 330};
  int durations[] = {250, 250, 400};
  for (int i = 0; i < 3; i++) {
    tone(PIN_BUZZER, notes[i], durations[i]);
    delay(durations[i] + 50);
  }
}