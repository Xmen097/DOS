#include <Arduino.h>
#include <EEPROM.h>
#include "define.h"

namespace eeprom {
  void setup() {
    for(int a=0; a<6; a++) {
      enter_code[a] = EEPROM.read(a);
    }
  }
  void update_code() {
    for(int a=0; a<6; a++) {
      EEPROM.write(a, entering_code[a]);
      enter_code[a] = entering_code[a];
    }
    keypad::clear_code();
    code_state = Code::correct;
    RGB::update();
    piezo::correct();
    code_timeout = millis();
  }
}
