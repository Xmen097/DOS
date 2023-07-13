#include <Arduino.h>

namespace RGB{
  extern unsigned long entering_time;
  extern const unsigned long entering_timeout;
  void update();
  void setup();
};
namespace keypad{
  void clear_code();
  void check_press();
};
namespace motor {
  extern unsigned long move_start_time;
  extern bool move_direction;
  extern int move_distance;
  void setup();
  void open();
  void close();
  void stop();
  void move();
};
namespace button {
  void setup();
  void check_press();
};
namespace piezo {
  void setup();
  void incorrect();
  void correct();
  void key();
  void open();
  void stop();
};
namespace eeprom {
  void setup();
  void update_code();
};
namespace receiver {
  void setup();
  void receive();
};
namespace lock {
  void setup();
  void open();
  void close();
  void unlocked_counter();
};

void code_timeout_check(); 
void open_door();

#define keypad_coll_1 A3
#define keypad_coll_2 A2
#define keypad_coll_3 A1
#define keypad_row_1 10
#define keypad_row_2 12
#define keypad_row_3 9 
#define keypad_row_4 13

#define RGB_R 6
#define RGB_G 5
#define RGB_B 11

#define motor_1 A4
#define motor_2 A5

#define receiver_pin 2

#define lock_pin 7

#define piezo_pin 3

#define button_pin 4


#define resolution 26


enum class Code {idle, entering, correct, incorrect, admin, admin_entering};
extern Code code_state;
extern byte entering_code[6];
extern byte code_position;
extern unsigned long code_timeout;
extern unsigned long last_button_press;

const char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
const byte rowPins[4] = {keypad_row_1, keypad_row_2, keypad_row_3, keypad_row_4};
const byte colPins[3] = {keypad_coll_1, keypad_coll_2, keypad_coll_3};

const int master_code[6] = {2, 6, 0, 2, 1, 0};
extern int enter_code[6]; // code will be read from eeprom
const int empty_code[6] = {0, 0, 0, 0, 0, 0};
const unsigned long open_time = 800; // How long will the lock be open
const unsigned long admin_time = 30000;
const unsigned long no_button_timeout = 10000; //How long can no button be pressed for the code to persist
