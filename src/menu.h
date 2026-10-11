#ifndef MENU_H
#define MENU_H

#include "game.h"

// Menu types
typedef enum {
    MENU_NONE,
    MENU_TITLE,
    MENU_MAIN,
    MENU_LOAD,
    MENU_SAVE,
    MENU_CONFIG,
    MENU_CHOICE,
    MENU_HUD,
    MENU_STATUS,
    MENU_GIFT,
    MENU_ACTION
} MenuID;

// Menu item
typedef struct {
    char text[32];
    uint8_t type;        // 0=action, 1=submenu, 2=toggle, 3=value
    int8_t param;        // Action ID, submenu ID, etc.
    bool enabled;
    bool visible;
} MenuItem;

// Menu state
typedef struct {
    MenuID id;
    MenuItem items[12];
    uint8_t item_count;
    uint8_t selected;
    uint8_t scroll_offset;
    uint8_t max_visible;
    char title[32];
    void (*on_select)(uint8_t index);
    void (*on_cancel)(void);
    void (*on_render)(void);
} Menu;

extern Menu g_menu;

// Menu functions
void menu_init(void);
void menu_open(MenuID id);
void menu_close(void);
void menu_update(void);
void menu_render(void);
void menu_move_selection(int8_t delta);
void menu_confirm(void);
void menu_cancel(void);

// Specific menu handlers
void menu_title_update(void);
void menu_title_render(void);
void menu_main_update(void);
void menu_main_render(void);
void menu_load_update(void);
void menu_load_render(void);
void menu_save_update(void);
void menu_save_render(void);
void menu_config_update(void);
void menu_config_render(void);
void menu_choice_update(void);
void choice_render(void);
void menu_hud_render(void);
void menu_status_render(void);
void menu_save_load_prepare(void);

// Menu callbacks
void on_new_game(uint8_t idx);
void on_load_game(uint8_t idx);
void on_config(uint8_t idx);
void on_quit(uint8_t idx);
void on_save_slot(uint8_t idx);
void on_load_slot(uint8_t idx);
void on_text_speed(uint8_t idx);
void on_auto_mode(uint8_t idx);
void on_skip_mode(uint8_t idx);

#endif // MENU_H