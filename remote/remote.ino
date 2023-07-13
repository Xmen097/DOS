#define F_CPU 1000000UL  //oscilator frequency
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include <VirtualWire.h>

#define transmitter_pin 3
#define button_pin 1
#define rotary_pin_1 0
#define rotary_pin_2 4
#define rotary_button 2

#define salt "apzy16h0"
#define resolution 26 // needs to be divisible by 2

volatile byte button_flag = 0;
volatile byte rotary_button_flag = 0;
volatile byte rotary_state = 0;
volatile bool rotary_direction = false;
volatile int rotary_position = resolution / 2;

void arm_watchdog ()
{
  WDTCR |= bit ( WDCE ) | bit ( WDE ) | bit ( WDIF ); // allow changes to prescaler, disable reset, clear existing interrupt
  WDTCR |= bit ( WDIE ) | bit ( WDP3 )| bit ( WDP0 ); // set WDIE ( Interrupt only, no Reset ) and 1 second TimeOut                                             
  wdt_reset (); // start the wdt countdown
}

void sleep() { 
  power_all_disable();
  cli();  // interrupts off to not mess with wdt
  arm_watchdog();  // activate the wdt countdown
  sleep_enable ();  // enable sleep
  sei ();  // allow interrupts
  sleep_cpu(); // enter sleep mode

  sleep_disable(); // disable sleep immediately after waking up
  power_all_enable(); // enable all modules 
}

void setup()
{
  cli(); //disable interrupts during setup
  vw_set_ptt_inverted(true);
  vw_setup(1000); // 1000bps
  vw_set_tx_pin(transmitter_pin); // transmitter config

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(rotary_button, INPUT);
  pinMode(rotary_pin_1, INPUT);
  pinMode(rotary_pin_2, INPUT);

  GIMSK |= (1 << PCIE);   // enable pin change interrupt
  PCMSK |= B10111; // enable PCINT on pins 0,1,2,4

  // sleep mode configuration
  MCUCR |= (1 << SM1);
  MCUCR &= ~(1 << SM0); // set Power-down sleep mode
  MCUCR |= (1 << BODS); // disable BOD
  
  sei(); //enable interrupts globaly
}

void loop() {
  if (button_flag == 2 || rotary_button_flag == 2) {
    GIMSK &= ~(1 << PCIE);   // disable pin change interrupt
    if (button_flag == 2) {
      char out[1] = {resolution + 1};
      vw_send((uint8_t *)out, 1); // the very last code is reserved as open code
      vw_wait_tx(); // wait until the whole message is send
      _delay_ms(100);
      button_flag = !digitalRead(button_pin);
    } else if (rotary_button_flag == 2) { // rotary_button_flag == 2
      char out[1] = {rotary_position};
      vw_send((uint8_t *)out, 1);
      vw_wait_tx(); // wait until the whole message is send
      _delay_ms(100);
      rotary_button_flag = !digitalRead(rotary_button);
      rotary_position = resolution / 2;  //reset rotary position
    }
    GIMSK |= (1 << PCIE);   // enable pin change interrupt
  }
  sleep(); // enter sleep mode after one loop execution
}

ISR(PCINT0_vect) {
  if (!digitalRead(button_pin) && button_flag == 0) {
    button_flag = 2;
  } else if (digitalRead(button_pin) && button_flag == 1) {
    button_flag = 0;
  } else if (!digitalRead(rotary_button) && rotary_button_flag == 0) {
    rotary_button_flag = 2;
  } else if (digitalRead(rotary_button) && rotary_button_flag == 1) {
    rotary_button_flag = 0;
  } else {
    bool pin1 = digitalRead(rotary_pin_1);
    bool pin2 = digitalRead(rotary_pin_2);

    if (pin1 && pin2) {
      if (rotary_state == B11) { // both bits are 1
        rotary_position = max(0, min(resolution, rotary_position + rotary_direction * 2 - 1)); // -1 or +1 restrained to <0, resolution>
      }
      rotary_direction = 0;
      rotary_state = 0;
    } else if (!pin1 && !pin2) {
      rotary_state &= B01; // set second bit to zero and leave first bit as is
    } else {
      bool current_direction = !pin1;
      if (!rotary_state) {
        rotary_direction = current_direction;
        rotary_state = 1;
      } else if (rotary_direction != current_direction) {
        rotary_state |= B10;
      }
    }
  }
}

ISR(WDT_vect) {
  wdt_disable ();
  rotary_position = resolution / 2;
}
