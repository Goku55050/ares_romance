#include "scenes.h"
#include "game.h"
#include "text.h"
#include "characters.h"
#include "input.h"
#include "tonc_stub.h"
#include <string.h>

SceneState g_scene = {SCENE_NONE, 0, 0, false, false};

// Scene definitions with suggestive but not explicit content
const Scene g_scenes[] = {
    // Intro
    {SCENE_INTRO, "bg_academy", NULL, {0}, {0}, false, false, 0,
     {"Welcome to the Arcane Academy...", "You are Ares, a student with unusual potential.", "Three paths await, each with its own danger... and desire.", NULL, NULL, NULL, NULL, NULL}, 3, NULL, NULL},
    
    // Daily scenes
    {SCENE_DAY_MORNING, "bg_dorm", NULL, {0}, {0}, false, false, 120,
     {"Morning light filters through the window.", "Another day of training begins.", NULL, NULL, NULL, NULL, NULL, NULL}, 2, NULL, NULL},
    {SCENE_DAY_NOON, "bg_courtyard", NULL, {0}, {0}, false, false, 120,
     {"The courtyard buzzes with activity.", "Students practice spells and swordplay.", NULL, NULL, NULL, NULL, NULL, NULL}, 2, NULL, NULL},
    {SCENE_DAY_EVENING, "bg_garden", NULL, {0}, {0}, false, false, 120,
     {"Evening shadows stretch across the gardens.", "A peaceful moment before night falls.", NULL, NULL, NULL, NULL, NULL, NULL}, 2, NULL, NULL},
    {SCENE_DAY_NIGHT, "bg_dorm_night", NULL, {0}, {0}, false, false, 120,
     {"Night settles over the academy.", "Time to rest... or sneak out?", NULL, NULL, NULL, NULL, NULL, NULL}, 2, NULL, NULL},
    
    // Date scenes
    {SCENE_DATE_KIRA, "bg_training", "cg_kira_date", {0}, {0}, true, false, 0,
     {"Kira: \"Hmph. Don't think this means anything.\"", "Her face flushes as you spar together.", "\"...You're not terrible. For a novice.\"", "The tension between you crackles like lightning.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_DATE_LUNA, "bg_observatory", "cg_luna_date", {0}, {0}, true, false, 0,
     {"Luna: \"The stars align strangely tonight.\"", "She leans closer, silver hair catching moonlight.", "\"Your fate... intertwines with mine. Interesting.\"", "Her fingers brush yours. Cold, yet electric.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_DATE_ROSA, "bg_garden", "cg_rosa_date", {0}, {0}, true, false, 0,
     {"Rosa: \"The night-blooming jasmine... it's beautiful.\"", "She tucks a flower behind your ear.", "\"I... I wanted you to have this.\"", "Her hand trembles. Your hearts race in unison.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    
    // Confession scenes
    {SCENE_CONFESSION_KIRA, "bg_rooftop", "cg_kira_confess", {0}, {0}, true, false, 0,
     {"Kira: \"I... damn it. I can't stop thinking about you.\"", "She grabs your collar, pulling you close.", "\"Don't you dare leave me. Ever.\"", "The kiss tastes of ash and cinnamon. Fire.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_CONFESSION_LUNA, "bg_library", "cg_luna_confess", {0}, {0}, true, false, 0,
     {"Luna: \"My calculations failed to predict... this.\"", "She closes her book, setting it aside deliberately.", "\"You are the variable I cannot solve. The one I don't want to.\"", "Her lips find yours. Tasting of old parchment and starlight.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_CONFESSION_ROSA, "bg_chapel", "cg_rosa_confess", {0}, {0}, true, false, 0,
     {"Rosa: \"I've prayed for courage. Every night.\"", "Tears shine in her eyes, reflected in candlelight.", "\"Ares... I love you. More than I can say.\"", "Gentle. Reverent. A promise sealed in silence.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    
    // Intimate scenes (suggestive, fade to black)
    {SCENE_INTIMATE_KIRA, "bg_dorm_night", "cg_kira_intimate", {0}, {0}, true, true, 0,
     {"The room is warm. Shadows dance on walls.", "Kira's hands trace paths of fire on your skin.", "\"Say my name. Only mine.\"", "The world narrows to breath and heartbeat.", "Morning finds you entwined, marks fading on shoulders.", NULL, NULL, NULL}, 5, NULL, NULL},
    {SCENE_INTIMATE_LUNA, "bg_observatory", "cg_luna_intimate", {0}, {0}, true, true, 0,
     {"Starlight paints her pale skin in silver.", "Luna's control shatters, piece by piece.", "\"Show me what the stars cannot see.\"", "Arcane sigils flare where skin meets skin.", "Dawn breaks. She traces formulas on your back.", NULL, NULL, NULL}, 5, NULL, NULL},
    {SCENE_INTIMATE_ROSA, "bg_garden", "cg_rosa_intimate", {0}, {0}, true, true, 0,
     {"Moonlit petals rain around you.", "Rosa's shyness blooms into something fierce.", "\"I want all of you. Every part.\"", "Thorns and roses. Pleasure edged with devotion.", "Sunrise. She sleeps against you, smiling.", NULL, NULL, NULL}, 5, NULL, NULL},
    
    // Endings
    {SCENE_ENDING_KIRA, "bg_rooftop", "cg_kira_ending", {0}, {0}, true, false, 0,
     {"Years later. She's captain of the guard.", "You wait at the academy gates.", "\"Took you long enough, idiot.\"", "Her smile could melt steel. You're home.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_ENDING_LUNA, "bg_tower", "cg_luna_ending", {0}, {0}, true, false, 0,
     {"Archmage Luna. The youngest in history.", "You're her anchor to humanity.", "\"My research is complete. The subject: us.\"", "Infinite futures. You choose this one.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_ENDING_ROSA, "bg_garden", "cg_rosa_ending", {0}, {0}, true, false, 0,
     {"Head Healer Rosa. The garden flourishes.", "Children laugh among eternal roses.", "\"Our family... it's perfect.\"", "Peace. Finally, completely, peace.", NULL, NULL, NULL, NULL}, 4, NULL, NULL},
    {SCENE_ENDING_HAREM, "bg_academy", "cg_harem_ending", {0}, {0}, true, true, 0,
     {"Three women. One impossible choice... resolved.", "They found a way. Together.", "\"We share. That's the only way.\"", "The academy whispers. You don't care.", "Four souls. One bond. Eternal.", NULL, NULL, NULL, NULL}, 5, NULL, NULL},
};

const uint8_t g_scene_count = sizeof(g_scenes) / sizeof(Scene);

void scenes_init(void) {
    g_scene.current = SCENE_NONE;
    g_scene.timer = 0;
    g_scene.text_index = 0;
    g_scene.waiting = false;
    g_scene.fading = false;
}

void scene_trigger(SceneID scene) {
    if (scene >= g_scene_count) return;
    
    g_scene.current = scene;
    g_scene.timer = 0;
    g_scene.text_index = 0;
    g_scene.waiting = false;
    g_scene.fading = true;
    
    // Fade in
    g_game.fading_in = true;
    g_game.fade_alpha = 0;
    
    // Load background
    scene_load_bg(scene);
    
    // Load CG if present
    const Scene* s = &g_scenes[scene];
    if (s->has_cg) {
        scene_load_cg(scene);
    }
    
    // Call on_start
    if (s->on_start) s->on_start();
    
    // Show first text line
    if (s->text_count > 0) {
        text_show_dialogue(ARCHETYPE_NARRATOR, s->text_lines[0]);
        g_scene.text_index = 1;
    }
    
    // Mark CG as seen
    if (s->has_cg) {
        // Unlock in gallery
    }
}

void scene_update(void) {
    if (g_scene.current == SCENE_NONE) return;
    
    const Scene* s = &g_scenes[g_scene.current];
    
    // Handle fade in
    if (g_game.fading_in) return;
    
    // Auto-advance timer
    if (s->duration_frames > 0) {
        g_scene.timer++;
        if (g_scene.timer >= s->duration_frames) {
            scene_advance();
        }
    }
    
    // Handle text display
    if (!g_text.is_typing && g_text.wait_for_input) {
        if (key_hit(KEY_A) || key_hit(KEY_B)) {
            scene_advance();
        }
    }
}

void scene_render(void) {
    // Background and CG rendered via BG layers
    // Text rendered by text system
    
    // Show skip hint for intimate scenes
    const Scene* s = &g_scenes[g_scene.current];
    if (s->is_intimate) {
        text_draw_string("START: Skip", 160, 144, COL_GRAY);
    }
}

bool scene_is_playing(void) {
    return g_scene.current != SCENE_NONE;
}

void scene_advance(void) {
    const Scene* s = &g_scenes[g_scene.current];
    
    if (g_scene.text_index < s->text_count) {
        // Show next text line
        text_show_dialogue(ARCHETYPE_NARRATOR, s->text_lines[g_scene.text_index]);
        g_scene.text_index++;
    } else {
        // Scene ended
        g_game.fading_out = true;
        g_scene.fading = true;
        
        // Call on_end
        if (s->on_end) s->on_end();
        
        // Handle post-scene logic
        switch (g_scene.current) {
            case SCENE_INTRO:
                game_change_state(STATE_PLAYING);
                break;
            case SCENE_CONFESSION_KIRA:
            case SCENE_CONFESSION_LUNA:
            case SCENE_CONFESSION_ROSA:
                // Lock route
                break;
            case SCENE_INTIMATE_KIRA:
            case SCENE_INTIMATE_LUNA:
            case SCENE_INTIMATE_ROSA:
                // Max intimacy
                break;
            case SCENE_ENDING_KIRA:
            case SCENE_ENDING_LUNA:
            case SCENE_ENDING_ROSA:
            case SCENE_ENDING_HAREM:
                game_change_state(STATE_ENDING);
                break;
        }
        
        // Wait for fade out
        if (g_game.fade_alpha == 0) {
            g_scene.current = SCENE_NONE;
            g_scene.fading = false;
        }
    }
}

void scene_load_bg(SceneID id) {
    // Load background into BG3
    // In production: DMA copy from ROM to VRAM
    // Placeholder: just set a solid color
    BG_PALETTE[240] = RGB15(4, 4, 12); // Dark blue for night scenes
    BG_PALETTE[241] = RGB15(8, 6, 4);  // Warm for day
    BG_PALETTE[242] = RGB15(2, 8, 4);  // Green for garden
}

void scene_load_cg(SceneID id) {
    // Load CG into BG2 (portrait layer) or OBJ
    // In production: 256-color bitmap or tiled
    // Placeholder: mark as loaded
}