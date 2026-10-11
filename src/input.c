#include "input.h"
#include "input.h"
#include "tonc_stub.h"

uint16_t g_key_current = 0;
uint16_t g_key_previous = 0;
uint16_t g_key_held = 0;
uint16_t g_key_released = 0;

uint8_t g_key_repeat_counter = 0;
uint8_t g_key_repeat_delay = 30;
uint8_t g_key_repeat_rate = 6;

void input_init(void) {
    g_key_current = 0;
    g_key_previous = 0;
    g_key_held = 0;
    g_key_released = 0;
    g_key_repeat_counter = 0;
}

void input_update(void) {
    g_key_previous = g_key_current;
    g_key_current = ~REG_KEYINPUT & KEY_ANY;
    
    g_key_held = g_key_current & g_key_previous;
    g_key_released = g_key_previous & ~g_key_current;
    
    // Key repeat for held keys
    if (g_key_held) {
        g_key_repeat_counter++;
        if (g_key_repeat_counter >= g_key_repeat_delay) {
            g_key_repeat_counter = g_key_repeat_delay - g_key_repeat_rate;
        }
    } else {
        g_key_repeat_counter = 0;
    }
}

bool key_down(uint16_t key) {
    return (g_key_current & key) != 0;
}

bool key_hit(uint16_t key) {
    return (g_key_current & key) && !(g_key_previous & key);
}

bool key_up(uint16_t key) {
    return !(g_key_current & key) && (g_key_previous & key);
}

bool key_held(uint16_t key, uint8_t delay, uint8_t rate) {
    if (!(g_key_held & key)) return false;
    
    if (g_key_repeat_counter >= delay) {
        return (g_key_repeat_counter % rate) == 0;
    }
    return false;
}