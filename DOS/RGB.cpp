#include <Arduino.h>
#include "define.h"

namespace RGB {
    unsigned long entering_time = 0;
    const unsigned long entering_timeout = 200;

    void code_entering() {
      if(entering_time == 0)
        entering_time = millis();
      digitalWrite(RGB_R, LOW);
      digitalWrite(RGB_G, LOW);
      digitalWrite(RGB_B, HIGH);
    }
    void code_idle(byte B_intensity) {
      if(0 == entering_code[0] && 0 == entering_code[1] && 0 == entering_code[2] && 0 == entering_code[3] && 0 == entering_code[4] && 0 == entering_code[5]) {
        digitalWrite(RGB_R, LOW);
        digitalWrite(RGB_G, LOW);
        digitalWrite(RGB_B, LOW);
      } else {
        digitalWrite(RGB_R, LOW);
        digitalWrite(RGB_G, LOW);
        analogWrite(RGB_B, B_intensity);
      }
    }
    void code_incorrect() {
      digitalWrite(RGB_R, HIGH);
      digitalWrite(RGB_G, LOW);
      digitalWrite(RGB_B, LOW);
    }
    void code_correct() {
      digitalWrite(RGB_R, LOW);
      digitalWrite(RGB_G, HIGH);
      digitalWrite(RGB_B, LOW);
    }

    void code_admin(byte R_intensity, byte G_intensity, byte B_intensity) {
      analogWrite(RGB_R, R_intensity);
      analogWrite(RGB_G, G_intensity);
      analogWrite(RGB_B, B_intensity);
    }

    void code_admin_entering() {
      if(entering_time == 0)
        entering_time = millis();
      digitalWrite(RGB_R, HIGH);
      digitalWrite(RGB_G, HIGH);
      digitalWrite(RGB_B, HIGH);
    }

    
    void update() {
      switch(code_state) {
        case Code::idle:
          code_idle(1);
          break;
        case Code::entering:
          code_entering();
          break;
        case Code::correct:
          code_correct();
          break;
        case Code::incorrect:
          code_incorrect();
          break;
        case Code::admin:
          code_admin(25, 25, 25);
          break;
        case Code::admin_entering:
          code_admin_entering();
          break;
      }
    }
    void setup() {
      pinMode(RGB_R, OUTPUT);
      pinMode(RGB_G, OUTPUT);
      pinMode(RGB_B, OUTPUT);
    }
};
