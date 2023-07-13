#include <Arduino.h>
#include "define.h"
#include "notes.h"

namespace piezo {
  void setup() {
    pinMode(piezo_pin, OUTPUT);  
  }
  void incorrect() {
    analogWrite(piezo_pin, 20);
    delay(100);
    analogWrite(piezo_pin, 0);  
  }
  void key() {
    tone(piezo_pin, NOTE_E5, 1000/8);
    delay(1000/8);
  }
  void correct() {
    tone(piezo_pin, NOTE_A4, 1000/8);
    delay(1000/8);
    tone(piezo_pin, NOTE_A5, 1000/8);
    delay(1000/8);
  }
  void open() {
    tone(piezo_pin, NOTE_A4);
  }
  void stop() {
    noTone(piezo_pin);
  }
}
