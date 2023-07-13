#include <Arduino.h>
#include "define.h"

namespace button {
  void setup() {
    pinMode(button_pin, INPUT);
  }
  void check_press() {
    if(!digitalRead(button_pin))  {
      open_door();  
      delay(500);
    }
  }
}
