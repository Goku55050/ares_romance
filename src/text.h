#ifndef TEXT_H
#define TEXT_H

#include "game.h"

// Text system constants
#define TEXT_BG_MAP_BASE 30
#define TEXT_TILE_BASE 0
#define TEXT_MAX_LINES 4
#define TEXT_CHARS_PER_LINE 38
#define TEXT_BOX_X 8
#define TEXT_BOX_Y 112
#define TEXT_BOX_W 224
#define TEXT_BOX_H 40

// Font: 8x8 tiles, 1bpp -> 256 chars in 256 tiles
#define FONT_TILES 256

// Portrait positions
#define PORTRAIT_LEFT_X 8
#define PORTRAIT_RIGHT_X 160
#define PORTRAIT_Y 16
#define PORTRAIT_W 64
#define PORTRAIT_H 80

// Text colors (palette indices)
#define COL_WHITE 1
#define COL_GRAY 2
#define COL_RED 3
#define COL_BLUE 4
#define COL_GREEN 5
#define COL_YELLOW 6
#define COL_PINK 7
#define COL_ORANGE 8

// Text state
typedef struct {
    char buffer[TEXT_MAX_LINES][TEXT_CHARS_PER_LINE + 1];
    uint8_t line_count;
    uint8_t current_char;
    uint8_t current_line;
    uint8_t display_speed; // Frames per char
    uint8_t frame_counter;
    bool is_typing;
    bool wait_for_input;
    uint8_t portrait_left;
    uint8_t portrait_right;
    uint8_t name_color;
} TextState;

extern TextState g_text;

// Functions
void text_init(void);
void text_clear(void);
void text_set_portrait(uint8_t left_id, uint8_t right_id);
void text_show_dialogue(Archetype speaker, const char* text);
void text_append(const char* text);
void text_update(void);
void text_render(void);
void text_draw_char(char c, uint8_t x, uint8_t y, uint8_t color);
void text_draw_string(const char* str, uint8_t x, uint8_t y, uint8_t color);
uint8_t text_get_width(const char* str);
void text_wait_input(void);
bool text_is_done(void);
void text_skip(void);
void text_speed_set(uint8_t speed);

#endif // TEXT_H