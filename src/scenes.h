#ifndef SCENES_H
#define SCENES_H

#include "game.h"

// Scene/CG system
typedef struct {
    SceneID id;
    const char* bg_name;        // Background asset
    const char* cg_name;        // CG asset (optional)
    uint8_t bg_palette[16];     // Custom palette
    uint8_t cg_palette[16];
    bool has_cg;
    bool is_intimate;           // Suggestive content
    uint16_t duration_frames;   // Auto-advance time (0 = wait for input)
    const char* text_lines[8];  // Accompanying text
    uint8_t text_count;
    void (*on_start)(void);
    void (*on_end)(void);
} Scene;

// Scene state
typedef struct {
    SceneID current;
    uint16_t timer;
    uint8_t text_index;
    bool waiting;
    bool fading;
} SceneState;

extern SceneState g_scene;

// Scene functions
void scenes_init(void);
void scene_trigger(SceneID scene);
void scene_update(void);
void scene_render(void);
bool scene_is_playing(void);
void scene_advance(void);
void scene_load_bg(SceneID id);
void scene_load_cg(SceneID id);

// Predefined scenes
extern const Scene g_scenes[];
extern const uint8_t g_scene_count;

#endif // SCENES_H