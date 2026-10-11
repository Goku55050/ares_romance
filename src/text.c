#include "text.h"
#include "game.h"
#include "tonc_stub.h"
#include <string.h>

TextState g_text;

// Forward declaration
static void text_scroll_up(void);

// Simple 8x8 font data (ASCII 32-126)
// In production, load from font.gbf or generate
static const uint8_t font_data[96 * 8] = {
    // Space (32)
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    // ! (33)
    0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x00,
    // " (34)
    0x36,0x36,0x36,0x00,0x00,0x00,0x00,0x00,
    // # (35)
    0x36,0x36,0x7F,0x36,0x7F,0x36,0x36,0x00,
    // $ (36)
    0x18,0x3C,0x60,0x3C,0x06,0x3C,0x18,0x00,
    // % (37)
    0x63,0x63,0x66,0x0C,0x18,0x30,0x46,0x46,
    // & (38)
    0x1C,0x36,0x1C,0x38,0x1C,0x26,0x1C,0x00,
    // ' (39)
    0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
    // ( (40)
    0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00,
    // ) (41)
    0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00,
    // * (42)
    0x00,0x18,0x0C,0x7E,0x0C,0x18,0x00,0x00,
    // + (43)
    0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
    // , (44)
    0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,
    // - (45)
    0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,
    // . (46)
    0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,
    // / (47)
    0x06,0x0C,0x18,0x30,0x60,0xC0,0x00,0x00,
    // 0 (48)
    0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00,
    // 1 (49)
    0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00,
    // 2 (50)
    0x3C,0x66,0x06,0x0C,0x18,0x30,0x7E,0x00,
    // 3 (51)
    0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00,
    // 4 (52)
    0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00,
    // 5 (53)
    0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00,
    // 6 (54)
    0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00,
    // 7 (55)
    0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00,
    // 8 (56)
    0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00,
    // 9 (57)
    0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00,
    // : (58)
    0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00,
    // ; (59)
    0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30,
    // < (60)
    0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00,
    // = (61)
    0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00,
    // > (62)
    0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00,
    // ? (63)
    0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00,
    // @ (64)
    0x3C,0x66,0x6E,0x7A,0x7E,0x60,0x3C,0x00,
    // A (65)
    0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00,
    // B (66)
    0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00,
    // C (67)
    0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00,
    // D (68)
    0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00,
    // E (69)
    0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00,
    // F (70)
    0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00,
    // G (71)
    0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00,
    // H (72)
    0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00,
    // I (73)
    0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,
    // J (74)
    0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00,
    // K (75)
    0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00,
    // L (76)
    0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00,
    // M (77)
    0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00,
    // N (78)
    0x66,0x76,0x7E,0x6E,0x66,0x66,0x66,0x00,
    // O (79)
    0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00,
    // P (80)
    0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00,
    // Q (81)
    0x3C,0x66,0x66,0x66,0x6E,0x6C,0x3E,0x00,
    // R (82)
    0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00,
    // S (83)
    0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00,
    // T (84)
    0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,
    // U (85)
    0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00,
    // V (86)
    0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,
    // W (87)
    0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00,
    // X (88)
    0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00,
    // Y (89)
    0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00,
    // Z (90)
    0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00,
    // [ (91)
    0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,
    // \ (92)
    0x00,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,
    // ] (93)
    0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,
    // ^ (94)
    0x18,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,
    // _ (95)
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,
};

void text_init(void) {
    // Clear VRAM for text BG
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(TEXT_BG_MAP_BASE);
    for (int i = 0; i < 32*32; i++) map[i] = 0;
    
    // Load font into tile memory (4bpp, so each char = 32 bytes = 2 tiles)
    // We'll use 1bpp font rendered to 4bpp tiles at runtime
    // For simplicity, use 8x8 1bpp tiles directly
    uint16_t* tiles = (uint16_t*)BG_TILE_BASE_ADR(TEXT_TILE_BASE);
    for (int c = 0; c < 96; c++) {
        for (int y = 0; y < 8; y++) {
            uint8_t row = font_data[c * 8 + y];
            uint16_t tile_row = 0;
            for (int x = 0; x < 8; x++) {
                if (row & (0x80 >> x)) {
                    tile_row |= (COL_WHITE << (x * 2));
                }
            }
            tiles[c * 4 + y] = tile_row;
            tiles[c * 4 + y + 8] = 0; // Second tile (empty for 8x8)
        }
    }
    
    // Setup text box BG palette
    BG_PALETTE[COL_WHITE] = RGB15(31, 31, 31);
    BG_PALETTE[COL_GRAY] = RGB15(20, 20, 20);
    BG_PALETTE[COL_RED] = RGB15(31, 10, 10);
    BG_PALETTE[COL_BLUE] = RGB15(10, 10, 31);
    BG_PALETTE[COL_GREEN] = RGB15(10, 31, 10);
    BG_PALETTE[COL_YELLOW] = RGB15(31, 31, 10);
    BG_PALETTE[COL_PINK] = RGB15(31, 15, 25);
    BG_PALETTE[COL_ORANGE] = RGB15(31, 20, 0);
    
    // Text box background palette
    BG_PALETTE[14] = RGB15(0, 0, 8);      // Dark blue bg
    BG_PALETTE[15] = RGB15(2, 2, 12);     // Border
    
    memset(&g_text, 0, sizeof(TextState));
    g_text.display_speed = 2;
}

void text_clear(void) {
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(TEXT_BG_MAP_BASE);
    // Clear text area (bottom 5 rows)
    for (int y = 20; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            map[y * 32 + x] = 0;
        }
    }
    // Clear portrait area
    for (int y = 2; y < 12; y++) {
        for (int x = 1; x < 9; x++) {
            map[y * 32 + x] = 0;
            map[y * 32 + (32 - 9 + x)] = 0;
        }
    }
    memset(g_text.buffer, 0, sizeof(g_text.buffer));
    g_text.line_count = 0;
    g_text.current_char = 0;
    g_text.current_line = 0;
    g_text.is_typing = false;
    g_text.wait_for_input = false;
}

void text_set_portrait(uint8_t left_id, uint8_t right_id) {
    g_text.portrait_left = left_id;
    g_text.portrait_right = right_id;
    // Portraits would be loaded into BG2 tile/map memory
    // Placeholder: just mark for rendering
}

void text_show_dialogue(Archetype speaker, const char* text) {
    text_clear();
    
    // Set name color by archetype
    switch (speaker) {
        case ARCHETYPE_MC: g_text.name_color = COL_BLUE; break;
        case ARCHETYPE_TSUNDERE: g_text.name_color = COL_RED; break;
        case ARCHETYPE_KUUDERE: g_text.name_color = COL_BLUE; break;
        case ARCHETYPE_DANDERE: g_text.name_color = COL_PINK; break;
        case ARCHETYPE_NARRATOR: g_text.name_color = COL_GRAY; break;
        default: g_text.name_color = COL_WHITE; break;
    }
    
    // Draw name
    const char* name = archetype_name(speaker);
    text_draw_string(name, TEXT_BOX_X + 4, TEXT_BOX_Y + 2, g_text.name_color);
    text_draw_string(":", TEXT_BOX_X + 4 + strlen(name) * 8, TEXT_BOX_Y + 2, COL_WHITE);
    
    // Start typing effect
    g_text.is_typing = true;
    g_text.current_char = 0;
    g_text.current_line = 0;
    g_text.frame_counter = 0;
    
    // Store full text for typing
    strncpy(g_text.buffer[0], text, TEXT_CHARS_PER_LINE * TEXT_MAX_LINES);
    g_text.buffer[0][TEXT_CHARS_PER_LINE * TEXT_MAX_LINES] = '\0';
    
    // Draw text box background
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(TEXT_BG_MAP_BASE);
    for (int y = 20; y < 30; y++) {
        for (int x = 1; x < 31; x++) {
            map[y * 32 + x] = 14 | (0 << 12); // Palette 14 (dark blue)
        }
    }
    // Border
    for (int x = 1; x < 31; x++) {
        map[20 * 32 + x] = 15 | (0 << 12);
        map[29 * 32 + x] = 15 | (0 << 12);
    }
    for (int y = 20; y < 30; y++) {
        map[y * 32 + 1] = 15 | (0 << 12);
        map[y * 32 + 30] = 15 | (0 << 12);
    }
}

void text_append(const char* text) {
    // Append to current buffer
    int len = strlen(g_text.buffer[0]);
    strncat(g_text.buffer[0], text, TEXT_CHARS_PER_LINE * TEXT_MAX_LINES - len - 1);
}

void text_update(void) {
    if (!g_text.is_typing) return;
    
    g_text.frame_counter++;
    if (g_text.frame_counter >= g_text.display_speed) {
        g_text.frame_counter = 0;
        
        char* text = g_text.buffer[0];
        int len = strlen(text);
        
        if (g_text.current_char < len) {
            // Calculate position
            int char_x = TEXT_BOX_X + 4 + (g_text.current_char % TEXT_CHARS_PER_LINE) * 8;
            int char_y = TEXT_BOX_Y + 18 + (g_text.current_char / TEXT_CHARS_PER_LINE) * 16;
            
            if (char_y > TEXT_BOX_Y + 40) {
                // Scroll up
                text_scroll_up();
                char_y = TEXT_BOX_Y + 18 + (TEXT_MAX_LINES - 1) * 16;
            }
            
            text_draw_char(text[g_text.current_char], char_x, char_y, COL_WHITE);
            g_text.current_char++;
        } else {
            g_text.is_typing = false;
            g_text.wait_for_input = true;
        }
    }
}

void text_render(void) {
    // Text is rendered directly to BG map in text_update/text_show_dialogue
    // This function can handle portraits, name boxes, etc.
    
    // Draw "press A" indicator if waiting
    if (g_text.wait_for_input && !g_text.is_typing) {
        // Blinking arrow at bottom right
        if ((g_game.frame_count / 30) % 2 == 0) {
            text_draw_string("▶", TEXT_BOX_X + 200, TEXT_BOX_Y + 28, COL_YELLOW);
        }
    }
}

void text_draw_char(char c, uint8_t x, uint8_t y, uint8_t color) {
    if (c < 32 || c > 126) return;
    
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(TEXT_BG_MAP_BASE);
    uint8_t tile_x = x / 8;
    uint8_t tile_y = y / 8;
    uint8_t tile_id = c - 32; // Font starts at space
    
    map[tile_y * 32 + tile_x] = tile_id | (color << 12);
}

void text_draw_string(const char* str, uint8_t x, uint8_t y, uint8_t color) {
    for (int i = 0; str[i] && i < TEXT_CHARS_PER_LINE; i++) {
        text_draw_char(str[i], x + i * 8, y, color);
    }
}

uint8_t text_get_width(const char* str) {
    return strlen(str) * 8;
}

void text_wait_input(void) {
    g_text.wait_for_input = true;
}

bool text_is_done(void) {
    return !g_text.is_typing && g_text.wait_for_input;
}

void text_skip(void) {
    if (g_text.is_typing) {
        // Instantly show all text
        char* text = g_text.buffer[0];
        int len = strlen(text);
        
        for (int i = g_text.current_char; i < len; i++) {
            int char_x = TEXT_BOX_X + 4 + (i % TEXT_CHARS_PER_LINE) * 8;
            int char_y = TEXT_BOX_Y + 18 + (i / TEXT_CHARS_PER_LINE) * 16;
            if (char_y <= TEXT_BOX_Y + 40) {
                text_draw_char(text[i], char_x, char_y, COL_WHITE);
            }
        }
        g_text.current_char = len;
        g_text.is_typing = false;
        g_text.wait_for_input = true;
    }
}

void text_speed_set(uint8_t speed) {
    g_text.display_speed = speed > 3 ? 3 : speed;
}

// Helper: scroll text up one line
void text_scroll_up(void) {
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(TEXT_BG_MAP_BASE);
    // Copy lines up
    for (int y = 21; y < 29; y++) {
        for (int x = 2; x < 30; x++) {
            map[(y-1) * 32 + x] = map[y * 32 + x];
        }
    }
    // Clear bottom line
    for (int x = 2; x < 30; x++) {
        map[28 * 32 + x] = 14 | (0 << 12);
    }
}