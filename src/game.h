#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>
#include "tonc_stub.h"

// Screen dimensions
#define SCREEN_W 240
#define SCREEN_H 160

// Game constants
#define MAX_HEROINES 3
#define MAX_AFFECTION 100
#define MAX_SCRIPT_LINES 500
#define MAX_CHOICES 4
#define SAVE_SLOTS 3
#define MAX_TEXT_CHARS 256

// Game states
typedef enum {
    STATE_BOOT,
    STATE_TITLE,
    STATE_MENU_MAIN,
    STATE_MENU_LOAD,
    STATE_MENU_SAVE,
    STATE_MENU_CONFIG,
    STATE_PLAYING,
    STATE_DIALOGUE,
    STATE_CHOICE,
    STATE_SCENE_CG,
    STATE_AFFECTION_CHECK,
    STATE_ENDING,
    STATE_GAME_OVER
} GameState;

// Heroine IDs
typedef enum {
    HEROINE_KIRA = 0,   // Fiery warrior, tsundere
    HEROINE_LUNA = 1,   // Mysterious mage, kuudere
    HEROINE_ROSA = 2,   // Gentle healer, dandere
    HEROINE_COUNT
} HeroineID;

// Character archetypes for dialogue
typedef enum {
    ARCHETYPE_MC,       // Ares
    ARCHETYPE_TSUNDERE, // Kira
    ARCHETYPE_KUUDERE,  // Luna
    ARCHETYPE_DANDERE,  // Rosa
    ARCHETYPE_NARRATOR,
    ARCHETYPE_SYSTEM
} Archetype;

// Scene types
typedef enum {
    SCENE_NONE,
    SCENE_INTRO,
    SCENE_DAY_MORNING,
    SCENE_DAY_NOON,
    SCENE_DAY_EVENING,
    SCENE_DAY_NIGHT,
    SCENE_DATE_KIRA,
    SCENE_DATE_LUNA,
    SCENE_DATE_ROSA,
    SCENE_CONFESSION_KIRA,
    SCENE_CONFESSION_LUNA,
    SCENE_CONFESSION_ROSA,
    SCENE_INTIMATE_KIRA,   // Suggestive, fade to black
    SCENE_INTIMATE_LUNA,
    SCENE_INTIMATE_ROSA,
    SCENE_ENDING_KIRA,
    SCENE_ENDING_LUNA,
    SCENE_ENDING_ROSA,
    SCENE_ENDING_HAREM,    // Secret route
    SCENE_COUNT
} SceneID;

// Dialogue line
typedef struct {
    Archetype speaker;
    char text[MAX_TEXT_CHARS];
    uint8_t portrait_id;  // Which portrait to show
    bool is_choice;
    uint8_t choice_count;
    char choices[MAX_CHOICES][64];
    int8_t choice_effects[MAX_CHOICES][MAX_HEROINES]; // Affection changes
    uint16_t next_line[MAX_CHOICES]; // Next line per choice
    uint16_t default_next;
    SceneID trigger_scene;
    bool ends_chapter;
} DialogueLine;

// Heroine data
typedef struct {
    HeroineID id;
    char name[16];
    uint8_t affection;          // 0-100
    uint8_t trust;              // 0-100 (unlocks deeper dialogue)
    uint8_t intimacy;           // 0-100 (suggestive content gate)
    uint16_t flags;             // Bit flags for events seen
    uint8_t route_progress;     // 0-10 (chapter)
    bool route_locked;          // True if other routes maxed
    uint8_t portrait_base;      // VRAM offset for portraits
    uint8_t sprite_id;          // OAM sprite index
} Heroine;

// Main character (Ares)
typedef struct {
    char name[16];
    uint8_t charm;      // Affects all affection gains
    uint8_t perception; // Unlocks hidden dialogue options
    uint8_t stamina;    // Limits daily actions
    uint16_t flags;     // Global story flags
    uint8_t day;        // Current day (1-30 per chapter)
    uint8_t chapter;    // Current chapter (1-10)
    uint8_t energy;     // Daily action points (0-100)
} MainChar;

// Save slot
typedef struct {
    char signature[4];        // "ARSV"
    uint16_t version;         // Save format version
    uint32_t playtime_frames; // Total frames played
    uint8_t slot_index;       // 0-2
    GameState state;
    MainChar mc;
    Heroine heroines[MAX_HEROINES];
    uint16_t current_line;
    SceneID last_scene;
    uint8_t seen_cgs[32];     // Bitfield of unlocked CGs
    uint8_t endings_unlocked; // Bitfield
    uint16_t checksum;
} SaveData;

// Global game state
typedef struct {
    GameState state;
    GameState prev_state;
    MainChar mc;
    Heroine heroines[MAX_HEROINES];
    DialogueLine script[MAX_SCRIPT_LINES];
    uint16_t script_count;
    uint16_t current_line;
    uint8_t text_speed;       // 0=instant, 1=fast, 2=normal, 3=slow
    bool auto_mode;
    bool skip_mode;
    uint8_t choice_selected;
    uint8_t choice_count;
    SceneID pending_scene;
    uint8_t fade_alpha;       // 0-16 for transitions
    bool fading_in;
    bool fading_out;
    uint32_t frame_count;
    uint16_t rng_seed;
} Game;

extern Game g_game;

// Function declarations
void game_init(void);
void game_update(void);
void game_render(void);
void game_change_state(GameState new_state);
void game_rng_seed(uint16_t seed);
uint16_t game_rng_next(void);
int game_rng_range(int min, int max);

// Affection helpers
void affection_change(HeroineID id, int8_t amount);
bool affection_check(HeroineID id, uint8_t threshold);
uint8_t affection_get(HeroineID id);
void affection_add_trust(HeroineID id, uint8_t amount);
void affection_add_intimacy(HeroineID id, uint8_t amount);

// Scene management
void scene_trigger(SceneID scene);
void scene_update(void);
void scene_render(void);
bool scene_is_playing(void);

// Script/dialogue
void script_load_chapter(uint8_t chapter);
void dialogue_advance(void);
void dialogue_render(void);
void choice_render(void);

// Save/load
bool save_game(uint8_t slot);
bool load_game(uint8_t slot);
void save_autosave(void);
bool save_slot_exists(uint8_t slot);

// Utility
const char* heroine_name(HeroineID id);
const char* archetype_name(Archetype a);

#endif // GAME_H