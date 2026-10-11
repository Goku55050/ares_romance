#ifndef SCRIPT_H
#define SCRIPT_H

#include "game.h"

// Script commands
typedef enum {
    CMD_DIALOGUE = 0,    // Speaker, text
    CMD_CHOICE = 1,      // Prompt, options, effects
    CMD_SCENE = 2,       // Trigger scene
    CMD_AFFECTION = 3,   // Modify affection
    CMD_FLAG = 4,        // Set/clear flag
    CMD_JUMP = 5,        // Jump to line
    CMD_WAIT = 6,        // Wait for input
    CMD_AUTO = 7,        // Auto-advance
    CMD_MUSIC = 8,       // Change BGM
    CMD_SFX = 9,         // Play sound
    CMD_PORTRAIT = 10,   // Show/hide portrait
    CMD_BG = 11,         // Change background
    CMD_END = 12,        // End chapter
    CMD_CALL = 13,       // Call subroutine
    CMD_RETURN = 14,     // Return from subroutine
    CMD_IF = 15,         // Conditional
    CMD_ELSE = 16,
    CMD_ENDIF = 17,
    CMD_VAR = 18,        // Set variable
    CMD_RANDOM = 19,     // Random branch
    CMD_ACTION = 20,     // Perform action
} ScriptCmd;

// Compiled script instruction
typedef struct {
    uint8_t cmd;
    uint8_t param1;
    uint8_t param2;
    int16_t param3;
    int16_t param4;
    char text[128];      // For dialogue/choice text
} ScriptInstr;

extern ScriptInstr g_compiled_script[];
extern uint16_t g_script_size;

// Functions
void script_load_chapter(uint8_t chapter);
void script_execute(uint16_t line);
bool script_is_choice(uint16_t line);
uint8_t script_get_choice_count(uint16_t line);
const char* script_get_choice_text(uint16_t line, uint8_t choice);
int8_t script_get_choice_effect(uint16_t line, uint8_t choice, HeroineID heroine);
uint16_t script_get_choice_next(uint16_t line, uint8_t choice);

#endif // SCRIPT_H