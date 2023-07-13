#include "define.h"
#include <avr/wdt.h>

Code code_state = Code::idle;
byte entering_code[6] = {0, 0, 0, 0, 0, 0};
byte code_position = 0;
unsigned long code_timeout;
unsigned long last_button_press = 0;
int enter_code[6]; // code will be read from eeprom

void setup() {
  eeprom::setup();
  receiver::setup();
  motor::setup();
  RGB::setup();
  lock::setup();
  piezo::setup();
  button::setup();
  piezo::correct();
  wdt_enable(WDTO_2S); // Enable WatchDog timer; If loop takes more than 2 seconds (it should never during normal operation), it will reset the controller.
}

void loop() {
  code_timeout_check();
  button::check_press();
  keypad::check_press();
  receiver::receive();
  motor::move();
  RGB::update();
  lock::unlocked_counter();
  wdt_reset(); // Reset the WatchDog timer
}
