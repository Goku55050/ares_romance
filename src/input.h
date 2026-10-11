#ifndef INPUT_H
#define INPUT_H

#include "tonc_stub.h"

// Key state tracking
extern uint16_t g_key_current;
extern uint16_t g_key_previous;
extern uint16_t g_key_held;
extern uint16_t g_key_released;

// Key repeat
extern uint8_t g_key_repeat_counter;
extern uint8_t g_key_repeat_delay;
extern uint8_t g_key_repeat_rate;

// Functions
void input_init(void);
void input_update(void);
bool key_down(uint16_t key);
bool key_hit(uint16_t key);
bool key_up(uint16_t key);
bool key_held(uint16_t key, uint8_t delay, uint8_t rate);

#endif // INPUT_H