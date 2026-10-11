#include "menu.h"
#include "game.h"
#include "text.h"
#include "save.h"
#include "characters.h"
#include "input.h"
#include "tonc_stub.h"
#include <string.h>
#include <stdio.h>

Menu g_menu;

static void menu_title_select(uint8_t idx) {
    switch (idx) {
        case 0: on_new_game(0); break;
        case 1: on_load_game(0); break;
        case 2: on_config(0); break;
        case 3: on_quit(0); break;
    }
}

static void menu_main_select(uint8_t idx) {
    switch (idx) {
        case 0: game_change_state(STATE_PLAYING); menu_close(); break;
        case 1: menu_open(MENU_SAVE); break;
        case 2: menu_open(MENU_LOAD); break;
        case 3: menu_open(MENU_STATUS); break;
        case 4: menu_open(MENU_CONFIG); break;
        case 5: game_change_state(STATE_TITLE); menu_open(MENU_TITLE); break;
    }
}

static void menu_load_select(uint8_t idx) {
    if (idx < SAVE_SLOTS) on_load_slot(idx);
    else menu_open(MENU_MAIN);
}

static void menu_save_select(uint8_t idx) {
    if (idx < SAVE_SLOTS) on_save_slot(idx);
    else menu_open(MENU_MAIN);
}

static void menu_config_select(uint8_t idx) {
    switch (idx) {
        case 0: on_text_speed(0); break;
        case 1: on_auto_mode(0); break;
        case 2: on_skip_mode(0); break;
        case 3: menu_open(MENU_MAIN); break;
    }
}

static void menu_status_select(uint8_t idx) {
    (void)idx;
    menu_open(MENU_MAIN);
}

void menu_init(void) {
    memset(&g_menu, 0, sizeof(Menu));
    g_menu.max_visible = 6;
}

void menu_open(MenuID id) {
    g_menu.id = id;
    g_menu.selected = 0;
    g_menu.scroll_offset = 0;
    g_menu.item_count = 0;
    g_menu.on_select = NULL;
    g_menu.on_cancel = NULL;
    g_menu.on_render = NULL;
    
    switch (id) {
        case MENU_TITLE:
            strcpy(g_menu.title, "ARES ROMANCE");
            strcpy(g_menu.items[0].text, "New Game");
            g_menu.items[0].type = 0; g_menu.items[0].param = 0; g_menu.items[0].enabled = true;
            strcpy(g_menu.items[1].text, "Load Game");
            g_menu.items[1].type = 0; g_menu.items[1].param = 1; g_menu.items[1].enabled = save_slot_exists(0) || save_slot_exists(1) || save_slot_exists(2);
            strcpy(g_menu.items[2].text, "Config");
            g_menu.items[2].type = 0; g_menu.items[2].param = 2; g_menu.items[2].enabled = true;
            strcpy(g_menu.items[3].text, "Quit");
            g_menu.items[3].type = 0; g_menu.items[3].param = 3; g_menu.items[3].enabled = true;
            g_menu.item_count = 4;
            g_menu.on_select = menu_title_select;
            g_menu.on_render = menu_title_render;
            break;
            
        case MENU_MAIN:
            strcpy(g_menu.title, "MAIN MENU");
            strcpy(g_menu.items[0].text, "Continue");
            g_menu.items[0].type = 0; g_menu.items[0].param = 0; g_menu.items[0].enabled = true;
            strcpy(g_menu.items[1].text, "Save");
            g_menu.items[1].type = 0; g_menu.items[1].param = 1; g_menu.items[1].enabled = true;
            strcpy(g_menu.items[2].text, "Load");
            g_menu.items[2].type = 0; g_menu.items[2].param = 2; g_menu.items[2].enabled = true;
            strcpy(g_menu.items[3].text, "Status");
            g_menu.items[3].type = 0; g_menu.items[3].param = 3; g_menu.items[3].enabled = true;
            strcpy(g_menu.items[4].text, "Config");
            g_menu.items[4].type = 0; g_menu.items[4].param = 4; g_menu.items[4].enabled = true;
            strcpy(g_menu.items[5].text, "Quit to Title");
            g_menu.items[5].type = 0; g_menu.items[5].param = 5; g_menu.items[5].enabled = true;
            g_menu.item_count = 6;
            g_menu.on_select = menu_main_select;
            g_menu.on_render = menu_main_render;
            break;
            
        case MENU_LOAD:
            strcpy(g_menu.title, "LOAD GAME");
            for (int i = 0; i < SAVE_SLOTS; i++) {
                char name[32];
                uint32_t time;
                uint8_t ch;
                save_get_slot_info(i, name, &time, &ch);
                sprintf(g_menu.items[i].text, "Slot %d: %s Ch.%d", i+1, name, ch);
                g_menu.items[i].type = 0; g_menu.items[i].param = i; g_menu.items[i].enabled = save_slot_exists(i);
            }
            strcpy(g_menu.items[3].text, "Back");
            g_menu.items[3].type = 0; g_menu.items[3].param = -1; g_menu.items[3].enabled = true;
            g_menu.item_count = 4;
            g_menu.on_select = menu_load_select;
            g_menu.on_render = menu_load_render;
            break;
            
        case MENU_SAVE:
            strcpy(g_menu.title, "SAVE GAME");
            for (int i = 0; i < SAVE_SLOTS; i++) {
                char name[32];
                uint32_t time;
                uint8_t ch;
                save_get_slot_info(i, name, &time, &ch);
                sprintf(g_menu.items[i].text, "Slot %d: %s", i+1, save_slot_exists(i) ? name : "EMPTY");
                g_menu.items[i].type = 0; g_menu.items[i].param = i; g_menu.items[i].enabled = true;
            }
            strcpy(g_menu.items[3].text, "Back");
            g_menu.items[3].type = 0; g_menu.items[3].param = -1; g_menu.items[3].enabled = true;
            g_menu.item_count = 4;
            g_menu.on_select = menu_save_select;
            g_menu.on_render = menu_save_render;
            break;
            
        case MENU_CONFIG:
            strcpy(g_menu.title, "CONFIG");
            sprintf(g_menu.items[0].text, "Text Speed: %s", 
                g_game.text_speed == 0 ? "Instant" : g_game.text_speed == 1 ? "Fast" : g_game.text_speed == 2 ? "Normal" : "Slow");
            g_menu.items[0].type = 3; g_menu.items[0].param = 0; g_menu.items[0].enabled = true;
            sprintf(g_menu.items[1].text, "Auto Mode: %s", g_game.auto_mode ? "ON" : "OFF");
            g_menu.items[1].type = 2; g_menu.items[1].param = 1; g_menu.items[1].enabled = true;
            sprintf(g_menu.items[2].text, "Skip Seen: %s", g_game.skip_mode ? "ON" : "OFF");
            g_menu.items[2].type = 2; g_menu.items[2].param = 2; g_menu.items[2].enabled = true;
            strcpy(g_menu.items[3].text, "Back");
            g_menu.items[3].type = 0; g_menu.items[3].param = -1; g_menu.items[3].enabled = true;
            g_menu.item_count = 4;
            g_menu.on_select = menu_config_select;
            g_menu.on_render = menu_config_render;
            break;
            
        case MENU_CHOICE:
            break;
            
        case MENU_HUD:
            g_menu.on_render = menu_hud_render;
            break;
            
        case MENU_STATUS:
            strcpy(g_menu.title, "STATUS");
            g_menu.on_render = menu_status_render;
            g_menu.on_select = menu_status_select;
            break;
    }
    
    for (int i = 0; i < g_menu.item_count; i++) {
        g_menu.items[i].visible = true;
    }
}

void menu_close(void) {
    g_menu.id = MENU_NONE;
    g_menu.item_count = 0;
}

void menu_update(void) {
    if (g_menu.id == MENU_NONE) return;
    
    if (key_hit(KEY_UP)) {
        menu_move_selection(-1);
    } else if (key_hit(KEY_DOWN)) {
        menu_move_selection(1);
    } else if (key_hit(KEY_A)) {
        menu_confirm();
    } else if (key_hit(KEY_B)) {
        menu_cancel();
    } else if (key_hit(KEY_LEFT) || key_hit(KEY_RIGHT)) {
        if (g_menu.selected < g_menu.item_count) {
            MenuItem* item = &g_menu.items[g_menu.selected];
            if (item->type == 2) {
                menu_confirm();
            }
        }
    }
}

void menu_render(void) {
    if (g_menu.id == MENU_NONE) return;
    
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(28);
    for (int i = 0; i < 32*32; i++) map[i] = 0;
    
    text_draw_string(g_menu.title, 80, 16, COL_YELLOW);
    
    int start_y = 48;
    for (int i = 0; i < g_menu.item_count && i < g_menu.max_visible; i++) {
        int idx = g_menu.scroll_offset + i;
        if (idx >= g_menu.item_count) break;
        
        MenuItem* item = &g_menu.items[idx];
        if (!item->visible) continue;
        
        uint8_t color = item->enabled ? (idx == g_menu.selected ? COL_YELLOW : COL_WHITE) : COL_GRAY;
        int y = start_y + i * 20;
        
        if (idx == g_menu.selected && item->enabled) {
            text_draw_string(">", 20, y, COL_YELLOW);
        }
        
        text_draw_string(item->text, 40, y, color);
    }
    
    if (g_menu.on_render) g_menu.on_render();
}

void menu_move_selection(int8_t delta) {
    int new_sel = g_menu.selected + delta;
    
    for (int i = 0; i < g_menu.item_count; i++) {
        if (new_sel < 0) new_sel = g_menu.item_count - 1;
        if (new_sel >= g_menu.item_count) new_sel = 0;
        
        if (g_menu.items[new_sel].enabled && g_menu.items[new_sel].visible) {
            g_menu.selected = new_sel;
            break;
        }
        new_sel += delta;
    }
    
    if (g_menu.selected < g_menu.scroll_offset) {
        g_menu.scroll_offset = g_menu.selected;
    } else if (g_menu.selected >= g_menu.scroll_offset + g_menu.max_visible) {
        g_menu.scroll_offset = g_menu.selected - g_menu.max_visible + 1;
    }
}

void menu_confirm(void) {
    if (g_menu.selected < g_menu.item_count && g_menu.on_select) {
        g_menu.on_select(g_menu.selected);
    }
}

void menu_cancel(void) {
    if (g_menu.on_cancel) {
        g_menu.on_cancel();
    } else {
        switch (g_menu.id) {
            case MENU_LOAD:
            case MENU_SAVE:
            case MENU_CONFIG:
            case MENU_STATUS:
                menu_open(MENU_MAIN);
                break;
            case MENU_MAIN:
                game_change_state(STATE_PLAYING);
                menu_close();
                break;
            case MENU_TITLE:
                break;
        }
    }
}

void menu_title_update(void) {
    menu_update();
    if (key_hit(KEY_START)) {
        on_new_game(0);
    }
}

void menu_title_render(void) {
    text_draw_string("A Tale of Bonds and Destiny", 48, 40, COL_PINK);
    text_draw_string("Press START or A", 72, 130, COL_GRAY);
    text_draw_string("v1.0.0", 200, 144, COL_GRAY);
}

void menu_main_update(void) {
    menu_update();
}

void menu_main_render(void) {
    char buf[32];
    sprintf(buf, "Day %d  Chapter %d", g_game.mc.day, g_game.mc.chapter);
    text_draw_string(buf, 120, 10, COL_WHITE);
    sprintf(buf, "Energy: %d/100", g_game.mc.energy);
    text_draw_string(buf, 120, 26, COL_GREEN);
    
    for (int i = 0; i < MAX_HEROINES; i++) {
        sprintf(buf, "%s: %d", heroine_name(i), g_game.heroines[i].affection);
        text_draw_string(buf, 10, 120 + i * 12, 
            g_game.heroines[i].affection > 50 ? COL_PINK : COL_WHITE);
    }
}

void menu_load_update(void) {
    menu_update();
}

void menu_load_render(void) {
    for (int i = 0; i < SAVE_SLOTS; i++) {
        int y = 48 + i * 20;
        if (save_slot_exists(i)) {
            char name[32], time_str[16];
            uint32_t time;
            uint8_t ch;
            save_get_slot_info(i, name, &time, &ch);
            sprintf(time_str, "%dh%02dm", time / 3600, (time % 3600) / 60);
            text_draw_string(time_str, 180, y, COL_GRAY);
        }
    }
}

void menu_save_update(void) {
    menu_update();
}

void menu_save_render(void) {
    text_draw_string("Select slot to save", 60, 120, COL_GRAY);
    
    for (int i = 0; i < SAVE_SLOTS; i++) {
        int y = 48 + i * 20;
        if (save_slot_exists(i)) {
            char name[32];
            uint32_t time;
            uint8_t ch;
            save_get_slot_info(i, name, &time, &ch);
            sprintf(name, "Ch.%d Day%d", ch, g_game.mc.day);
            text_draw_string(name, 180, y, COL_GRAY);
        }
    }
}

void menu_config_update(void) {
    menu_update();
}

void menu_config_render(void) {
    sprintf(g_menu.items[0].text, "Text Speed: %s", 
        g_game.text_speed == 0 ? "Instant" : g_game.text_speed == 1 ? "Fast" : g_game.text_speed == 2 ? "Normal" : "Slow");
    sprintf(g_menu.items[1].text, "Auto Mode: %s", g_game.auto_mode ? "ON" : "OFF");
    sprintf(g_menu.items[2].text, "Skip Seen: %s", g_game.skip_mode ? "ON" : "OFF");
}

void menu_choice_update(void) {
}

void choice_render(void) {
    uint16_t* map = (uint16_t*)BG_MAP_BASE_ADR(28);
    int box_y = 20;
    for (int y = box_y; y < box_y + 12; y++) {
        for (int x = 4; x < 28; x++) {
            map[y * 32 + x] = 14 | (0 << 12);
        }
    }
    
    for (int i = 0; i < g_game.choice_count; i++) {
        int y = box_y * 8 + 8 + i * 20;
        uint8_t color = (i == g_game.choice_selected) ? COL_YELLOW : COL_WHITE;
        if (i == g_game.choice_selected) {
            text_draw_string(">", 40, y, COL_YELLOW);
        }
        text_draw_string(g_game.script[g_game.current_line].choices[i], 56, y, color);
    }
}

void menu_hud_render(void) {
    char buf[32];
    sprintf(buf, "Ch.%d Day %d", g_game.mc.chapter, g_game.mc.day);
    text_draw_string(buf, 8, 8, COL_WHITE);
    
    text_draw_string("EN", 8, 24, COL_GREEN);
    for (int i = 0; i < 10; i++) {
        if (i * 10 < g_game.mc.energy) {
            text_draw_string("█", 32 + i * 8, 24, COL_GREEN);
        } else {
            text_draw_string("░", 32 + i * 8, 24, COL_GRAY);
        }
    }
    
    for (int i = 0; i < MAX_HEROINES; i++) {
        int y = 48 + i * 16;
        uint8_t aff = g_game.heroines[i].affection;
        text_draw_string(heroine_name(i), 8, y, aff > 50 ? COL_PINK : COL_WHITE);
        
        for (int b = 0; b < 5; b++) {
            if (b * 20 < aff) {
                text_draw_string("█", 56 + b * 8, y, COL_PINK);
            } else {
                text_draw_string("░", 56 + b * 8, y, COL_GRAY);
            }
        }
    }
    
    text_draw_string("START: Menu", 140, 144, COL_GRAY);
}

void menu_status_render(void) {
    char buf[64];
    int y = 48;
    
    text_draw_string("ARES", 40, y, COL_BLUE); y += 20;
    sprintf(buf, "Charm: %d  Perception: %d  Stamina: %d", 
        g_game.mc.charm, g_game.mc.perception, g_game.mc.stamina);
    text_draw_string(buf, 20, y, COL_WHITE); y += 24;
    
    for (int i = 0; i < MAX_HEROINES; i++) {
        Heroine* h = &g_game.heroines[i];
        sprintf(buf, "%s - Aff: %d  Trust: %d  Int: %d", 
            h->name, h->affection, h->trust, h->intimacy);
        uint8_t color = h->affection > 70 ? COL_PINK : (h->affection > 30 ? COL_YELLOW : COL_WHITE);
        text_draw_string(buf, 20, y, color); y += 16;
    }
    
    text_draw_string("Press B to return", 60, 140, COL_GRAY);
}

void menu_save_load_prepare(void) {
    for (int i = 0; i < SAVE_SLOTS; i++) {
        char name[32];
        uint32_t time;
        uint8_t ch;
        save_get_slot_info(i, name, &time, &ch);
    }
}

void on_new_game(uint8_t idx) {
    (void)idx;
    game_init();
    game_change_state(STATE_PLAYING);
    menu_close();
    g_game.pending_scene = SCENE_INTRO;
}

void on_load_game(uint8_t idx) {
    (void)idx;
    menu_open(MENU_LOAD);
}

void on_config(uint8_t idx) {
    (void)idx;
    menu_open(MENU_CONFIG);
}

void on_quit(uint8_t idx) {
    (void)idx;
    game_change_state(STATE_TITLE);
    menu_open(MENU_TITLE);
}

void on_save_slot(uint8_t idx) {
    if (save_game(idx)) {
        text_show_dialogue(ARCHETYPE_SYSTEM, "Game saved successfully!");
    } else {
        text_show_dialogue(ARCHETYPE_SYSTEM, "Save failed!");
    }
    menu_open(MENU_MAIN);
}

void on_load_slot(uint8_t idx) {
    if (load_game(idx)) {
        game_change_state(STATE_PLAYING);
        menu_close();
    } else {
        text_show_dialogue(ARCHETYPE_SYSTEM, "Load failed!");
    }
}

void on_text_speed(uint8_t idx) {
    (void)idx;
    g_game.text_speed = (g_game.text_speed + 1) % 4;
    menu_open(MENU_CONFIG);
}

void on_auto_mode(uint8_t idx) {
    (void)idx;
    g_game.auto_mode = !g_game.auto_mode;
    menu_open(MENU_CONFIG);
}

void on_skip_mode(uint8_t idx) {
    (void)idx;
    g_game.skip_mode = !g_game.skip_mode;
    menu_open(MENU_CONFIG);
}