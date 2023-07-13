#include <Arduino.h>
#include "define.h"

#define motor_move_time 350

namespace motor {
unsigned long move_start_time;
bool move_direction;
int move_distance;

void setup() {
  pinMode(motor_1, OUTPUT);
  pinMode(motor_2, OUTPUT);
}

void open() {
  digitalWrite(motor_1, LOW);
  digitalWrite(motor_2, HIGH);
}
void close() {
  digitalWrite(motor_1, HIGH);
  digitalWrite(motor_2, LOW);
}
void dir_move(bool direction) {
  digitalWrite(motor_1, direction);
  digitalWrite(motor_2, !direction);
}
void stop() {
  digitalWrite(motor_1, LOW);
  digitalWrite(motor_2, LOW);
}

void move() {
  if (millis() - move_start_time < move_distance * motor_move_time) {
    dir_move(move_direction);
  } else {
    move_distance = 0;
    stop();
  }
}
}
