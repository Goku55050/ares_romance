#include "game.h"
#include "text.h"
#include "characters.h"
#include "save.h"
#include "menu.h"
#include "scenes.h"
#include "input.h"
#include <string.h>

// Global game instance
Game g_game;

// Interrupt handler
void irq_main(void) {
    // VBlank handled by BIOS
}

// Entry point
int main(void) {
    // Disable interrupts during init
    REG_IME = 0;
    irq_init(NULL);
    irq_add(II_VBLANK, irq_main);
    REG_IME = 1;

    // Initialize hardware
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
    
    // Enable BG0 for text, BG1 for UI, BG2 for portraits, BG3 for backgrounds
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_REG_32x32 | BG_PRIO(0); // Text
    REG_BG1CNT = BG_CBB(1) | BG_SBB(28) | BG_REG_32x32 | BG_PRIO(1); // UI/menus
    REG_BG2CNT = BG_CBB(2) | BG_SBB(26) | BG_REG_32x32 | BG_PRIO(2); // Portraits
    REG_BG3CNT = BG_CBB(3) | BG_SBB(24) | BG_REG_32x32 | BG_PRIO(3); // Backgrounds

    // Initialize subsystems
    text_init();
    characters_init();
    save_init();
    menu_init();
    scenes_init();
    input_init();

    // Seed RNG with timer
    game_rng_seed(REG_TM0D + g_game.frame_count);

    // Start at boot
    game_change_state(STATE_BOOT);

    // Main loop
    while (1) {
        VBlankIntrWait();
        
        input_update();
        game_update();
        game_render();
        
        g_game.frame_count++;
    }
    
    return 0;
}

void game_init(void) {
    // Reset game state
    g_game.state = STATE_BOOT;
    g_game.prev_state = STATE_BOOT;
    g_game.frame_count = 0;
    g_game.text_speed = 2;
    g_game.auto_mode = false;
    g_game.skip_mode = false;
    g_game.fade_alpha = 0;
    g_game.fading_in = false;
    g_game.fading_out = false;
    
    // Initialize Ares
    strcpy(g_game.mc.name, "Ares");
    g_game.mc.charm = 50;
    g_game.mc.perception = 50;
    g_game.mc.stamina = 50;
    g_game.mc.flags = 0;
    g_game.mc.day = 1;
    g_game.mc.chapter = 1;
    g_game.mc.energy = 100;
    
    // Initialize heroines
    for (int i = 0; i < MAX_HEROINES; i++) {
        g_game.heroines[i].affection = 0;
        g_game.heroines[i].trust = 0;
        g_game.heroines[i].intimacy = 0;
        g_game.heroines[i].flags = 0;
        g_game.heroines[i].route_progress = 0;
        g_game.heroines[i].route_locked = false;
    }
    
    // Load chapter 1 script
    script_load_chapter(1);
}

void game_update(void) {
    switch (g_game.state) {
        case STATE_BOOT:
            game_init();
            game_change_state(STATE_TITLE);
            break;
            
        case STATE_TITLE:
            menu_title_update();
            break;
            
        case STATE_MENU_MAIN:
            menu_main_update();
            break;
            
        case STATE_MENU_LOAD:
            menu_load_update();
            break;
            
        case STATE_MENU_SAVE:
            menu_save_update();
            break;
            
        case STATE_MENU_CONFIG:
            menu_config_update();
            break;
            
        case STATE_PLAYING:
            // Daily life gameplay
            if (g_game.mc.energy <= 0) {
                g_game.mc.day++;
                g_game.mc.energy = 100;
                if (g_game.mc.day > 30) {
                    g_game.mc.day = 1;
                    g_game.mc.chapter++;
                    script_load_chapter(g_game.mc.chapter);
                }
            }
            // Check for scene triggers
            if (g_game.pending_scene != SCENE_NONE) {
                scene_trigger(g_game.pending_scene);
                g_game.pending_scene = SCENE_NONE;
            }
            break;
            
        case STATE_DIALOGUE:
            dialogue_advance();
            break;
            
        case STATE_CHOICE:
            // Handled in menu_choice_update
            break;
            
        case STATE_SCENE_CG:
            scene_update();
            break;
            
        case STATE_AFFECTION_CHECK:
            // Check route locks, trigger confessions
            break;
            
        case STATE_ENDING:
            // Handle ending sequences
            break;
            
        case STATE_GAME_OVER:
            game_change_state(STATE_TITLE);
            break;
    }
    
    // Handle fade transitions
    if (g_game.fading_in) {
        g_game.fade_alpha = (g_game.fade_alpha < 16) ? g_game.fade_alpha + 1 : 16;
        if (g_game.fade_alpha >= 16) g_game.fading_in = false;
    }
    if (g_game.fading_out) {
        g_game.fade_alpha = (g_game.fade_alpha > 0) ? g_game.fade_alpha - 1 : 0;
        if (g_game.fade_alpha <= 0) g_game.fading_out = false;
    }
}

void game_render(void) {
    // Clear text BG each frame
    text_clear();
    
    switch (g_game.state) {
        case STATE_TITLE:
            menu_title_render();
            break;
        case STATE_MENU_MAIN:
            menu_main_render();
            break;
        case STATE_MENU_LOAD:
            menu_load_render();
            break;
        case STATE_MENU_SAVE:
            menu_save_render();
            break;
        case STATE_MENU_CONFIG:
            menu_config_render();
            break;
        case STATE_PLAYING:
            // Render daily UI
            menu_hud_render();
            break;
        case STATE_DIALOGUE:
            dialogue_render();
            break;
        case STATE_CHOICE:
            dialogue_render();
            choice_render();
            break;
        case STATE_SCENE_CG:
            scene_render();
            break;
        case STATE_ENDING:
            // Render ending
            break;
    }
    
    // Render fade overlay
    if (g_game.fade_alpha > 0 || g_game.fading_in || g_game.fading_out) {
        // Use BG3 for fade (palette 15 = black)
        REG_BLDCNT = BLD_BUILD(BLD_BG3, BLD_WHITE) | BLD_STD;
        REG_BLDY = g_game.fade_alpha;
    } else {
        REG_BLDCNT = 0;
    }
}

void game_change_state(GameState new_state) {
    g_game.prev_state = g_game.state;
    g_game.state = new_state;
    
    // State entry actions
    switch (new_state) {
        case STATE_DIALOGUE:
            g_game.current_line = 0;
            break;
        case STATE_MENU_LOAD:
        case STATE_MENU_SAVE:
            menu_save_load_prepare();
            break;
    }
}

void game_rng_seed(uint16_t seed) {
    g_game.rng_seed = seed ? seed : 1;
}

uint16_t game_rng_next(void) {
    // LCG: x = (1103515245 * x + 12345) % 2^16
    g_game.rng_seed = (g_game.rng_seed * 1103515245 + 12345) & 0xFFFF;
    return g_game.rng_seed;
}

int game_rng_range(int min, int max) {
    return min + (game_rng_next() % (max - min + 1));
}

const char* heroine_name(HeroineID id) {
    static const char* names[] = {
        "Kira",
        "Luna", 
        "Rosa"
    };
    return (id < MAX_HEROINES) ? names[id] : "Unknown";
}

const char* archetype_name(Archetype a) {
    static const char* names[] = {
        "Ares",
        "Kira",
        "Luna",
        "Rosa",
        "Narrator",
        "System"
    };
    return (a <= ARCHETYPE_SYSTEM) ? names[a] : "?";
}