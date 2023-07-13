#include <Arduino.h>
#include "define.h"

void print_code() {
  Serial.print(entering_code[0]);
  Serial.print(entering_code[1]);
  Serial.print(entering_code[2]);
  Serial.print(entering_code[3]);
  Serial.print(entering_code[4]);
  Serial.println(entering_code[5]);
}

void code_timeout_check() {
  if ((code_state == Code::correct && millis() - code_timeout >= open_time) || 
      (code_state == Code::incorrect && millis() - code_timeout >= open_time) || 
      (code_state == Code::admin && millis() - code_timeout >= admin_time)) {
    code_state = Code::idle;
    keypad::clear_code();
  } else if(code_state == Code::entering && millis() - RGB::entering_time >= RGB::entering_timeout) {
    code_state = Code::idle;
    RGB::entering_time = 0;
  } else if (code_state == Code::admin_entering && millis() - RGB::entering_time >= RGB::entering_timeout) {
    code_state = Code::admin;
    RGB::entering_time = 0;
  }

  if(millis() - last_button_press >= no_button_timeout) {
    keypad::clear_code();
  }
}

void open_door() {
  RGB::update();
  lock::open();
  motor::close();
  delay(300);
  motor::open();
  delay(500);
  motor::stop();
  lock::close();
}
