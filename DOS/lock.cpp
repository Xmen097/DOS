#include <Arduino.h>
#include "define.h"

namespace lock {
  int unlocked_count = 0;
  int unlocked_threshold = 10;
  unsigned long last_count = 0;

  void unlocked_counter() {
    if (millis() - last_count > 15000) {
      unlocked_count = max(0, unlocked_count-1);
      last_count = millis();
    }
  }
  
  void setup() {
    pinMode(lock_pin, OUTPUT);
  }
  void open() {
    if (unlocked_count < unlocked_threshold) {
      unlocked_count += 1;
      digitalWrite(lock_pin, HIGH);
      piezo::open();
    } else {
      piezo::incorrect();
    }
  }
  void close() {
    digitalWrite(lock_pin, LOW);
    piezo::stop();
  }
}
