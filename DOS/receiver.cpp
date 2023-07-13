#include <Arduino.h>
#include <VirtualWire.h>
#include "define.h"

namespace receiver {
  void setup() {  
    vw_set_ptt_inverted(true);
    vw_setup(1000);
    vw_set_rx_pin(receiver_pin);
    vw_rx_start();
  }

  void receive() {  
    uint8_t message[VW_MAX_MESSAGE_LEN];
    uint8_t delkaZpravy = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(message, &delkaZpravy)) {
      if(message[0] == resolution+1) {
        open_door();
      } else {
        motor::move_distance = abs(message[0] - resolution/2);
        motor::move_start_time = millis();
        motor::move_direction = message[0] - resolution/2 > 0 ? HIGH : LOW;
      }
    }
  }
}
