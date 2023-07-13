#include <Arduino.h>
#include <Key.h>
#include <Keypad.h>
#include "define.h"

namespace keypad {
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 3);
  void clear_code() {
    for(int a=0; a<6; a++) {
      entering_code[a] = 0;
    }
    code_position = 0;
  }
  
  void check_code() {
    if(master_code[0] == entering_code[0] && master_code[1] == entering_code[1] && master_code[2] == entering_code[2] && master_code[3] == entering_code[3] && master_code[4] == entering_code[4] && master_code[5] == entering_code[5]) {
      code_state = Code::admin;
      clear_code();
      code_timeout = millis();
    } else if(enter_code[0] == entering_code[0] && enter_code[1] == entering_code[1] && enter_code[2] == entering_code[2] && enter_code[3] == entering_code[3] && enter_code[4] == entering_code[4] && enter_code[5] == entering_code[5]) {
      code_state = Code::correct;
      RGB::update();
      piezo::correct();
      clear_code();
      code_timeout = millis();
      open_door();
    } else {
      code_state = Code::incorrect;
      RGB::update();
      piezo::incorrect();
      clear_code();
      code_timeout = millis();
    }
  } 
  
  void write_code(char key) {
    last_button_press = millis();
    if(key == '#') {
      if(code_state == Code::admin || code_state == Code::admin_entering) {
        eeprom::update_code();
      } else
        check_code();
    } else if(key == '*') {
      clear_code();
    } else {
        if(code_position < 6) {
          int key_number = key - '0';
          if(code_state == Code::admin || code_state == Code::admin_entering) {
            code_state = Code::admin_entering;  
          } else {
            code_state = Code::entering;  
          }
          RGB::update();
          piezo::key();
          entering_code[code_position++] = key_number;
        }
        if(code_position == 6){
          if(code_state == Code::admin || code_state == Code::admin_entering) {
            eeprom::update_code();
          } else
            check_code();
        }
    }
  }

  void check_press() {
    char key = keypad.getKey();
    if(key)
      write_code(key);
  }
}
