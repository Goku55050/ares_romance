#include "script.h"
#include "game.h"
#include "text.h"
#include "characters.h"
#include "scenes.h"
#include <string.h>

// Simple embedded script for Chapter 1
// In production, this would be loaded from binary data
static const ScriptInstr s_chapter1_script[] = {
    // Intro narration
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "The Arcane Academy. Where magic and steel shape destiny."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "You are Ares. A second-year student with... unusual heritage."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Three women cross your path. Each hides secrets. Each wants something from you."},
    
    // First choice - personality
    {CMD_CHOICE, 0, 4, 0, 0, "How do you present yourself?"},
    // Choice 0: Confident -> +charm
    {CMD_AFFECTION, HEROINE_KIRA, 2, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 1, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 0, 0, 0, ""},
    {CMD_VAR, 0, 1, 0, 0, ""}, // personality = confident
    // Choice 1: Reserved -> +perception
    {CMD_AFFECTION, HEROINE_KIRA, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 2, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 1, 0, 0, ""},
    {CMD_VAR, 0, 2, 0, 0, ""}, // personality = reserved
    // Choice 2: Playful -> +stamina
    {CMD_AFFECTION, HEROINE_KIRA, 1, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 2, 0, 0, ""},
    {CMD_VAR, 0, 3, 0, 0, ""}, // personality = playful
    // Choice 3: Mysterious -> +all slightly
    {CMD_AFFECTION, HEROINE_KIRA, 1, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 1, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 1, 0, 0, ""},
    {CMD_VAR, 0, 4, 0, 0, ""}, // personality = mysterious
    
    // Kira introduction
    {CMD_PORTRAIT, HEROINE_KIRA, 1, 0, 0, ""}, // Show Kira left
    {CMD_DIALOGUE, ARCHETYPE_TSUNDERE, 0, 0, 0, "\"You're in my way. Move.\""},
    {CMD_DIALOGUE, ARCHETYPE_TSUNDERE, 0, 0, 0, "\"...Hmph. New face. Don't think I'll go easy on you.\""},
    
    // Luna introduction
    {CMD_PORTRAIT, HEROINE_LUNA, 1, 0, 0, ""}, // Show Luna right
    {CMD_DIALOGUE, ARCHETYPE_KUUDERE, 0, 0, 0, "\"Interesting. Your mana signature is... irregular.\""},
    {CMD_DIALOGUE, ARCHETYPE_KUUDERE, 0, 0, 0, "\"I'm Luna. I study anomalies. You qualify.\""},
    
    // Rosa introduction
    {CMD_PORTRAIT, HEROINE_ROSA, 1, 0, 0, ""}, // Show Rosa left
    {CMD_DIALOGUE, ARCHETYPE_DANDERE, 0, 0, 0, "\"Oh! Um... h-hello. Are you lost?\""},
    {CMD_DIALOGUE, ARCHETYPE_DANDERE, 0, 0, 0, "\"I'm Rosa. I tend the medicinal gardens. It's... nice to meet you.\""},
    
    // Second choice - who to approach
    {CMD_CHOICE, 0, 3, 0, 0, "Who do you seek out first?"},
    // Choice 0: Kira
    {CMD_SCENE, SCENE_DATE_KIRA, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_KIRA, 5, 0, 0, ""},
    {CMD_JUMP, 20, 0, 0, 0, ""}, // Skip to Kira scene
    // Choice 1: Luna
    {CMD_SCENE, SCENE_DATE_LUNA, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 5, 0, 0, ""},
    {CMD_JUMP, 30, 0, 0, 0, ""}, // Skip to Luna scene
    // Choice 2: Rosa
    {CMD_SCENE, SCENE_DATE_ROSA, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 5, 0, 0, ""},
    {CMD_JUMP, 40, 0, 0, 0, ""}, // Skip to Rosa scene
    
    // Kira path
    {CMD_DIALOGUE, ARCHETYPE_TSUNDERE, 0, 0, 0, "\"Hah? You want to spar? Fine. But don't cry when you lose.\""},
    {CMD_DIALOGUE, ARCHETYPE_TSUNDERE, 0, 0, 0, "...Her stance shifts. Predatory. Excited."},
    {CMD_SCENE, SCENE_DATE_KIRA, 0, 0, 0, ""},
    {CMD_JUMP, 50, 0, 0, 0, ""},
    
    // Luna path
    {CMD_DIALOGUE, ARCHETYPE_KUUDERE, 0, 0, 0, "\"You seek knowledge? Very well. Meet me at the observatory. Midnight.\""},
    {CMD_DIALOGUE, ARCHETYPE_KUUDERE, 0, 0, 0, "Her eyes gleam. Calculating. Hungry."},
    {CMD_SCENE, SCENE_DATE_LUNA, 0, 0, 0, ""},
    {CMD_JUMP, 50, 0, 0, 0, ""},
    
    // Rosa path
    {CMD_DIALOGUE, ARCHETYPE_DANDERE, 0, 0, 0, "\"I... I'd like that. The night flowers bloom soon. Would you... watch with me?\""},
    {CMD_DIALOGUE, ARCHETYPE_DANDERE, 0, 0, 0, "She clasps her hands. Hopeful. Terrified."},
    {CMD_SCENE, SCENE_DATE_ROSA, 0, 0, 0, ""},
    
    // Converge - end of day
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "The day ends. But this is only the beginning."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Three paths. Three hearts. One destiny."},
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 2 - Developing bonds
static const ScriptInstr s_chapter2_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Week two. Routines form. Bonds deepen."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "But shadows stir at the academy's edges."},
    
    {CMD_CHOICE, 0, 4, 0, 0, "How do you spend your morning?"},
    // Train with Kira
    {CMD_ACTION, ACTION_TRAIN, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_KIRA, 3, 0, 0, ""},
    // Study with Luna
    {CMD_ACTION, ACTION_STUDY, 0, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 3, 0, 0, ""},
    // Garden with Rosa
    {CMD_ACTION, ACTION_TALK, HEROINE_ROSA, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 3, 0, 0, ""},
    // Solo
    {CMD_ACTION, ACTION_REST, 0, 0, 0, ""},
    
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 3 - Tensions rise
static const ScriptInstr s_chapter3_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "The festival approaches. Tensions rise."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Each woman asks for your time. You can't be everywhere."},
    
    {CMD_CHOICE, 0, 3, 0, 0, "Who do you promise the festival dance to?"},
    // Kira
    {CMD_AFFECTION, HEROINE_KIRA, 10, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, -5, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, -5, 0, 0, ""},
    {CMD_FLAG, 1, 1, 0, 0, ""}, // Promised to Kira
    // Luna
    {CMD_AFFECTION, HEROINE_KIRA, -5, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, 10, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, -5, 0, 0, ""},
    {CMD_FLAG, 1, 2, 0, 0, ""}, // Promised to Luna
    // Rosa
    {CMD_AFFECTION, HEROINE_KIRA, -5, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_LUNA, -5, 0, 0, ""},
    {CMD_AFFECTION, HEROINE_ROSA, 10, 0, 0, ""},
    {CMD_FLAG, 1, 3, 0, 0, ""}, // Promised to Rosa
    
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 4 - Festival
static const ScriptInstr s_chapter4_script[] = {
    {CMD_SCENE, SCENE_DAY_EVENING, 0, 0, 0, ""},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "The Moonlight Festival. Lanterns paint the night gold."},
    
    // Branch based on promise
    {CMD_IF, 1, 1, 0, 0, ""}, // If promised Kira
    {CMD_DIALOGUE, ARCHETYPE_TSUNDERE, 0, 0, 0, "\"You came. ...Good. I'd have hunted you down otherwise.\""},
    {CMD_SCENE, SCENE_DATE_KIRA, 0, 0, 0, ""},
    {CMD_ELSE, 0, 0, 0, 0, ""},
    {CMD_IF, 1, 2, 0, 0, ""}, // If promised Luna
    {CMD_DIALOGUE, ARCHETYPE_KUUDERE, 0, 0, 0, "\"Punctual. A rare virtue. Stay close. The stars are... active tonight.\""},
    {CMD_SCENE, SCENE_DATE_LUNA, 0, 0, 0, ""},
    {CMD_ELSE, 0, 0, 0, 0, ""},
    {CMD_IF, 1, 3, 0, 0, ""}, // If promised Rosa
    {CMD_DIALOGUE, ARCHETYPE_DANDERE, 0, 0, 0, "\"Y-you're here... I was so worried you'd forget...\"", ""},
    {CMD_SCENE, SCENE_DATE_ROSA, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 5 - Confessions begin
static const ScriptInstr s_chapter5_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Feelings can no longer be denied."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "But the academy holds darker secrets..."},
    
    // Check affection for confessions
    {CMD_IF, 0, HEROINE_KIRA, 85, 0, ""}, // If Kira affection >= 85
    {CMD_SCENE, SCENE_CONFESSION_KIRA, 0, 0, 0, ""},
    {CMD_FLAG, 2, 1, 0, 0, ""}, // Kira confessed
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_IF, 0, HEROINE_LUNA, 85, 0, ""},
    {CMD_SCENE, SCENE_CONFESSION_LUNA, 0, 0, 0, ""},
    {CMD_FLAG, 2, 2, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_IF, 0, HEROINE_ROSA, 85, 0, ""},
    {CMD_SCENE, SCENE_CONFESSION_ROSA, 0, 0, 0, ""},
    {CMD_FLAG, 2, 3, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 6 - Intimacy (suggestive)
static const ScriptInstr s_chapter6_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Lines blur. Boundaries dissolve."},
    
    {CMD_IF, 0, HEROINE_KIRA, 90, 0, ""},
    {CMD_IF, 1, 1, 0, 0, ""}, // If Kira confessed
    {CMD_SCENE, SCENE_INTIMATE_KIRA, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_IF, 0, HEROINE_LUNA, 90, 0, ""},
    {CMD_IF, 1, 2, 0, 0, ""},
    {CMD_SCENE, SCENE_INTIMATE_LUNA, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_IF, 0, HEROINE_ROSA, 90, 0, ""},
    {CMD_IF, 1, 3, 0, 0, ""},
    {CMD_SCENE, SCENE_INTIMATE_ROSA, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 7 - Crisis
static const ScriptInstr s_chapter7_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "The academy's barrier fails. Shadows pour in."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Your heritage awakens. Power surges."},
    {CMD_DIALOGUE, ARCHETYPE_MC, 0, 0, 0, "\"I won't let you take them. Any of you.\""},
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 8 - Climax
static const ScriptInstr s_chapter8_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "The final battle. Your choice decides everything."},
    
    {CMD_CHOICE, 0, 4, 0, 0, "Who fights beside you?"},
    // Kira only
    {CMD_AFFECTION, HEROINE_KIRA, 10, 0, 0, ""},
    {CMD_SCENE, SCENE_ENDING_KIRA, 0, 0, 0, ""},
    // Luna only
    {CMD_AFFECTION, HEROINE_LUNA, 10, 0, 0, ""},
    {CMD_SCENE, SCENE_ENDING_LUNA, 0, 0, 0, ""},
    // Rosa only
    {CMD_AFFECTION, HEROINE_ROSA, 10, 0, 0, ""},
    {CMD_SCENE, SCENE_ENDING_ROSA, 0, 0, 0, ""},
    // All three (harem route - requires all maxed)
    {CMD_IF, 0, 0, 100, 0, ""}, // Check all maxed (simplified)
    {CMD_SCENE, SCENE_ENDING_HAREM, 0, 0, 0, ""},
    {CMD_ELSE, 0, 0, 0, 0, ""},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "You stand alone. The cost is... everything."},
    {CMD_ENDIF, 0, 0, 0, 0, ""},
    
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 9 - Epilogue
static const ScriptInstr s_chapter9_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Peace returns. But changed. You are changed."},
    {CMD_END, 0, 0, 0, 0, ""},
};

// Chapter 10 - New Game+
static const ScriptInstr s_chapter10_script[] = {
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "Memories persist. New possibilities emerge."},
    {CMD_DIALOGUE, ARCHETYPE_NARRATOR, 0, 0, 0, "This time... you'll save everyone."},
    {CMD_END, 0, 0, 0, 0, ""},
};

// Pointers to chapter scripts
static const ScriptInstr* s_chapters[] = {
    s_chapter1_script,
    s_chapter2_script,
    s_chapter3_script,
    s_chapter4_script,
    s_chapter5_script,
    s_chapter6_script,
    s_chapter7_script,
    s_chapter8_script,
    s_chapter9_script,
    s_chapter10_script,
};

static const uint16_t s_chapter_sizes[] = {
    45,  // s_chapter1_script
    20,  // s_chapter2_script
    25,  // s_chapter3_script
    30,  // s_chapter4_script
    20,  // s_chapter5_script
    25,  // s_chapter6_script
    4,   // s_chapter7_script
    20,  // s_chapter8_script
    2,   // s_chapter9_script
    3,   // s_chapter10_script
};

// Current script pointers
static const ScriptInstr* g_current_script = NULL;
static uint16_t g_current_script_size = 0;

ScriptInstr g_compiled_script[500];
uint16_t g_script_size = 0;

void script_load_chapter(uint8_t chapter) {
    if (chapter == 0 || chapter > 10) chapter = 1;
    
    g_current_script = s_chapters[chapter - 1];
    g_current_script_size = s_chapter_sizes[chapter - 1];
    
    // Copy to working buffer for runtime modification
    memcpy(g_compiled_script, g_current_script, g_current_script_size * sizeof(ScriptInstr));
    g_script_size = g_current_script_size;
    
    g_game.current_line = 0;
}

void script_execute(uint16_t line) {
    if (line >= g_script_size) return;
    
    const ScriptInstr* instr = &g_compiled_script[line];
    
    switch (instr->cmd) {
        case CMD_DIALOGUE: {
            text_show_dialogue(instr->param1, instr->text);
            // Set portrait
            if (instr->param1 >= HEROINE_KIRA && instr->param1 <= HEROINE_ROSA) {
                text_set_portrait(instr->param1, 0);
            }
            break;
        }
        case CMD_CHOICE: {
            // Parse choice text (format: "Prompt|Opt1|Opt2|Opt3|Opt4")
            char* text = instr->text;
            char* parts[5];
            int part = 0;
            parts[part++] = text;
            for (char* p = text; *p && part < 5; p++) {
                if (*p == '|') {
                    *p = '\0';
                    parts[part++] = p + 1;
                }
            }
            
            g_game.choice_count = part - 1;
            // Store choices in script line for rendering
            for (int i = 0; i < g_game.choice_count; i++) {
                strncpy(g_game.script[line].choices[i], parts[i + 1], 63);
            }
            g_game.script[line].is_choice = true;
            g_game.choice_selected = 0;
            game_change_state(STATE_CHOICE);
            break;
        }
        case CMD_SCENE: {
            g_game.pending_scene = instr->param1;
            game_change_state(STATE_SCENE_CG);
            break;
        }
        case CMD_AFFECTION: {
            affection_change(instr->param1, instr->param2);
            break;
        }
        case CMD_FLAG: {
            if (instr->param2) {
                g_game.mc.flags |= (1 << instr->param1);
            } else {
                g_game.mc.flags &= ~(1 << instr->param1);
            }
            break;
        }
        case CMD_JUMP: {
            g_game.current_line = instr->param1 - 1; // -1 because dialogue_advance increments
            break;
        }
        case CMD_IF: {
            // Simple condition check
            bool condition = false;
            switch (instr->param1) {
                case 0: // Check affection
                    condition = affection_check(instr->param2, instr->param3);
                    break;
                case 1: // Check flag
                    condition = (g_game.mc.flags & (1 << instr->param2)) != 0;
                    break;
            }
            if (!condition) {
                // Skip to ELSE or ENDIF
                int depth = 1;
                for (uint16_t i = line + 1; i < g_script_size; i++) {
                    if (g_compiled_script[i].cmd == CMD_IF) depth++;
                    else if (g_compiled_script[i].cmd == CMD_ENDIF) {
                        depth--;
                        if (depth == 0) {
                            g_game.current_line = i;
                            break;
                        }
                    } else if (g_compiled_script[i].cmd == CMD_ELSE && depth == 1) {
                        g_game.current_line = i;
                        break;
                    }
                }
            }
            break;
        }
        case CMD_ELSE: {
            // Skip to ENDIF
            int depth = 1;
            for (uint16_t i = line + 1; i < g_script_size; i++) {
                if (g_compiled_script[i].cmd == CMD_IF) depth++;
                else if (g_compiled_script[i].cmd == CMD_ENDIF) {
                    depth--;
                    if (depth == 0) {
                        g_game.current_line = i;
                        break;
                    }
                }
            }
            break;
        }
        case CMD_ENDIF: {
            // Just a marker
            break;
        }
        case CMD_END: {
            // Chapter complete
            g_game.mc.chapter++;
            if (g_game.mc.chapter > 10) g_game.mc.chapter = 10;
            g_game.mc.day = 1;
            g_game.mc.energy = 100;
            script_load_chapter(g_game.mc.chapter);
            game_change_state(STATE_PLAYING);
            break;
        }
        case CMD_PORTRAIT: {
            if (instr->param2) {
                text_set_portrait(instr->param1, 0);
            } else {
                text_set_portrait(0, 0);
            }
            break;
        }
    }
}

bool script_is_choice(uint16_t line) {
    if (line >= g_script_size) return false;
    return g_compiled_script[line].cmd == CMD_CHOICE;
}

uint8_t script_get_choice_count(uint16_t line) {
    if (!script_is_choice(line)) return 0;
    // Count options in text
    uint8_t count = 0;
    for (char* p = g_compiled_script[line].text; *p; p++) {
        if (*p == '|') count++;
    }
    return count;
}

const char* script_get_choice_text(uint16_t line, uint8_t choice) {
    // Would need to parse - simplified
    return "Choice";
}

int8_t script_get_choice_effect(uint16_t line, uint8_t choice, HeroineID heroine) {
    // Effects stored in following AFFECTION instructions
    return 0;
}

uint16_t script_get_choice_next(uint16_t line, uint8_t choice) {
    // Next line after choice block
    return line + 1;
}