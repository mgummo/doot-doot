#include <M5Unified.hpp>

void play_doot_doot() {

    Serial.println("doot doot");

    M5.Speaker.setVolume(200);

    M5.Speaker.tone(1000, 150); // First doot
    delay(200);
    M5.Speaker.tone(1200, 150); // Second doot
    delay(200);
}