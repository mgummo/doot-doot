#include <M5Unified.hpp>

// Define the melody (frequency in Hz)
const int melody[] = {
  1319, 0, 1568, 0, 1760, 0, 2637, 2349, 2093
};

// Define note durations (in ms)
const int noteDurations[] = {
  150, 50, 150, 50, 150, 50, 200, 200, 300
};

const int numNotes = sizeof(melody) / sizeof(melody[0]);

void play_fanfare() {
  for (int i = 0; i < numNotes; i++) {
    int freq = melody[i];
    int duration = noteDurations[i];

    if (freq > 0) {
      M5.Speaker.tone(freq, duration);
    }
    delay(duration);
  }

  M5.Speaker.end();
}