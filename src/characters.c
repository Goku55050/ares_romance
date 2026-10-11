#include "characters.h"
#include "game.h"
#include "scenes.h"
#include <string.h>

// Action definitions
const Action g_actions[] = {
    // Talk actions
    {ACTION_TALK, HEROINE_KIRA, 10, {5, 0, 0}, {2, 0, 0}, {0, 0, 0}, 0, 0, 0,
     "Chat with Kira about training", false, SCENE_NONE},
    {ACTION_TALK, HEROINE_LUNA, 10, {0, 5, 0}, {0, 2, 0}, {0, 0, 0}, 0, 0, 0,
     "Discuss magic theory with Luna", false, SCENE_NONE},
    {ACTION_TALK, HEROINE_ROSA, 10, {0, 0, 5}, {0, 0, 2}, {0, 0, 0}, 0, 0, 0,
     "Share a quiet moment with Rosa", false, SCENE_NONE},
    
    // Gift actions (require items)
    {ACTION_GIFT, HEROINE_KIRA, 15, {10, -2, -2}, {3, 0, 0}, {1, 0, 0}, 0, 0, 0,
     "Give Kira a gift", true, SCENE_NONE},
    {ACTION_GIFT, HEROINE_LUNA, 15, {-2, 10, -2}, {0, 3, 0}, {0, 1, 0}, 0, 0, 0,
     "Give Luna a gift", true, SCENE_NONE},
    {ACTION_GIFT, HEROINE_ROSA, 15, {-2, -2, 10}, {0, 0, 3}, {0, 0, 1}, 0, 0, 0,
     "Give Rosa a gift", true, SCENE_NONE},
    
    // Date actions (high affection required)
    {ACTION_DATE, HEROINE_KIRA, 30, {15, -5, -5}, {5, 0, 0}, {5, 0, 0}, 0, 2, 0,
     "Spar with Kira at the training grounds", true, SCENE_DATE_KIRA},
    {ACTION_DATE, HEROINE_LUNA, 30, {-5, 15, -5}, {0, 5, 0}, {0, 5, 0}, 0, 0, 2,
     "Stargaze with Luna in the observatory", true, SCENE_DATE_LUNA},
    {ACTION_DATE, HEROINE_ROSA, 30, {-5, -5, 15}, {0, 0, 5}, {0, 0, 5}, 0, 0, 0,
     "Walk through the gardens with Rosa", true, SCENE_DATE_ROSA},
    
    // Self-improvement
    {ACTION_TRAIN, HEROINE_COUNT, 20, {2, 2, 2}, {0, 0, 0}, {0, 0, 0}, 5, 1, 0,
     "Train combat skills alone", false, SCENE_NONE},
    {ACTION_STUDY, HEROINE_COUNT, 20, {1, 1, 1}, {1, 1, 1}, {0, 0, 0}, 0, 0, 2,
     "Study magic and tactics", false, SCENE_NONE},
    {ACTION_REST, HEROINE_COUNT, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 30, 0, 0,
     "Rest and recover energy", false, SCENE_NONE},
    
    // Special (unlocked at high intimacy)
    {ACTION_SPECIAL, HEROINE_KIRA, 40, {20, -10, -10}, {10, 0, 0}, {15, 0, 0}, 0, 5, 0,
     "Intense sparring session with Kira...", true, SCENE_INTIMATE_KIRA},
    {ACTION_SPECIAL, HEROINE_LUNA, 40, {-10, 20, -10}, {0, 10, 0}, {0, 15, 0}, 0, 0, 5,
     "Share forbidden knowledge with Luna...", true, SCENE_INTIMATE_LUNA},
    {ACTION_SPECIAL, HEROINE_ROSA, 40, {-10, -10, 20}, {0, 0, 10}, {0, 0, 15}, 0, 0, 0,
     "Tender moment with Rosa under the moonlight...", true, SCENE_INTIMATE_ROSA},
};

const uint8_t g_action_count = sizeof(g_actions) / sizeof(Action);

// Gift definitions
const Gift g_gifts[] = {
    {GIFT_FLOWERS, "Wildflowers", "Simple but heartfelt", {3, 2, 5}, 50, false},
    {GIFT_SWEETS, "Honey Cakes", "Warm and comforting", {2, 3, 4}, 80, false},
    {GIFT_BOOK, "Ancient Tome", "Dense with forbidden knowledge", {1, 5, 2}, 120, false},
    {GIFT_WEAPON, "Training Blade", "Well-balanced for practice", {5, 1, 1}, 200, false},
    {GIFT_ACCESSORY, "Silver Pendant", "Catches the light beautifully", {3, 4, 3}, 150, false},
    {GIFT_SPECIAL_KIRA, "Dragon's Fang", "A fang from a fallen drake", {15, -5, -5}, 500, true},
    {GIFT_SPECIAL_LUNA, "Moonstone", "Glows with inner light", {-5, 15, -5}, 500, true},
    {GIFT_SPECIAL_ROSA, "Eternal Rose", "Never withers, never fades", {-5, -5, 15}, 500, true},
};

void characters_init(void) {
    // Kira - Fiery Warrior (Tsundere)
    g_game.heroines[HEROINE_KIRA].id = HEROINE_KIRA;
    strcpy(g_game.heroines[HEROINE_KIRA].name, "Kira");
    g_game.heroines[HEROINE_KIRA].portrait_base = 0;
    g_game.heroines[HEROINE_KIRA].sprite_id = 0;
    
    // Luna - Mysterious Mage (Kuudere)
    g_game.heroines[HEROINE_LUNA].id = HEROINE_LUNA;
    strcpy(g_game.heroines[HEROINE_LUNA].name, "Luna");
    g_game.heroines[HEROINE_LUNA].portrait_base = 10;
    g_game.heroines[HEROINE_LUNA].sprite_id = 1;
    
    // Rosa - Gentle Healer (Dandere)
    g_game.heroines[HEROINE_ROSA].id = HEROINE_ROSA;
    strcpy(g_game.heroines[HEROINE_ROSA].name, "Rosa");
    g_game.heroines[HEROINE_ROSA].portrait_base = 20;
    g_game.heroines[HEROINE_ROSA].sprite_id = 2;
}

void affection_change(HeroineID id, int8_t amount) {
    if (id >= MAX_HEROINES) return;
    
    Heroine* h = &g_game.heroines[id];
    int new_val = h->affection + amount;
    
    // Apply charm modifier
    if (amount > 0) {
        new_val += g_game.mc.charm / 25; // +1-2 per 25 charm
    }
    
    // Clamp
    if (new_val < 0) new_val = 0;
    if (new_val > MAX_AFFECTION) new_val = MAX_AFFECTION;
    
    h->affection = new_val;
    
    // Check milestones
    if (new_val >= 30 && !(h->flags & 0x01)) {
        h->flags |= 0x01; // First impression
        event_trigger_special(id, 1);
    }
    if (new_val >= 50 && !(h->flags & 0x02)) {
        h->flags |= 0x02; // Friend
        event_trigger_special(id, 2);
    }
    if (new_val >= 70 && !(h->flags & 0x04)) {
        h->flags |= 0x04; // Close
        event_trigger_special(id, 3);
    }
    if (new_val >= 85 && !(h->flags & 0x08)) {
        h->flags |= 0x08; // Intimate
        event_trigger_special(id, 4);
    }
    if (new_val >= 100 && !(h->flags & 0x10)) {
        h->flags |= 0x10; // Maxed
        event_trigger_special(id, 5);
    }
}

void affection_add_trust(HeroineID id, uint8_t amount) {
    if (id >= MAX_HEROINES) return;
    Heroine* h = &g_game.heroines[id];
    h->trust = (h->trust + amount > 100) ? 100 : h->trust + amount;
}

void affection_add_intimacy(HeroineID id, uint8_t amount) {
    if (id >= MAX_HEROINES) return;
    Heroine* h = &g_game.heroines[id];
    h->intimacy = (h->intimacy + amount > 100) ? 100 : h->intimacy + amount;
}

bool affection_check(HeroineID id, uint8_t threshold) {
    if (id >= MAX_HEROINES) return false;
    return g_game.heroines[id].affection >= threshold;
}

uint8_t affection_get(HeroineID id) {
    return (id < MAX_HEROINES) ? g_game.heroines[id].affection : 0;
}

uint8_t trust_get(HeroineID id) {
    return (id < MAX_HEROINES) ? g_game.heroines[id].trust : 0;
}

uint8_t intimacy_get(HeroineID id) {
    return (id < MAX_HEROINES) ? g_game.heroines[id].intimacy : 0;
}

void route_progress(HeroineID id) {
    if (id >= MAX_HEROINES) return;
    Heroine* h = &g_game.heroines[id];
    if (h->route_progress < 10) h->route_progress++;
    
    // Check for route lock (if one hits 8, others lock at 5)
    if (h->route_progress >= 8) {
        route_lock_others(id);
    }
}

bool route_is_locked(HeroineID id) {
    return (id < MAX_HEROINES) ? g_game.heroines[id].route_locked : false;
}

void route_lock_others(HeroineID chosen) {
    for (int i = 0; i < MAX_HEROINES; i++) {
        if (i != chosen && g_game.heroines[i].route_progress < 5) {
            g_game.heroines[i].route_locked = true;
        }
    }
}

HeroineID route_get_locked(void) {
    for (int i = 0; i < MAX_HEROINES; i++) {
        if (g_game.heroines[i].route_locked) return i;
    }
    return HEROINE_COUNT;
}

bool route_all_maxed(void) {
    for (int i = 0; i < MAX_HEROINES; i++) {
        if (g_game.heroines[i].affection < 100) return false;
    }
    return true;
}

bool action_perform(uint8_t action_index) {
    if (action_index >= g_action_count) return false;
    
    const Action* action = &g_actions[action_index];
    
    // Check energy
    if (g_game.mc.energy < action->energy_cost) return false;
    
    // Check affection requirements for dates/special
    if (action->type == ACTION_DATE) {
        if (!affection_check(action->target, 50)) return false;
    }
    if (action->type == ACTION_SPECIAL) {
        if (!affection_check(action->target, 85)) return false;
        if (intimacy_get(action->target) < 50) return false;
    }
    
    // Apply effects
    action_apply_effects(action);
    
    // Consume energy
    g_game.mc.energy -= action->energy_cost;
    if (g_game.mc.energy < 0) g_game.mc.energy = 0;
    
    // Gain stats
    g_game.mc.stamina = (g_game.mc.stamina + action->stamina_gain > 100) ? 100 : g_game.mc.stamina + action->stamina_gain;
    g_game.mc.charm = (g_game.mc.charm + action->charm_gain > 100) ? 100 : g_game.mc.charm + action->charm_gain;
    g_game.mc.perception = (g_game.mc.perception + action->perception_gain > 100) ? 100 : g_game.mc.perception + action->perception_gain;
    
    // Trigger scene if any
    if (action->unlocks_scene && action->scene_id != SCENE_NONE) {
        g_game.pending_scene = action->scene_id;
    }
    
    // Progress route
    if (action->target < MAX_HEROINES) {
        route_progress(action->target);
    }
    
    return true;
}

void action_apply_effects(const Action* action) {
    for (int i = 0; i < MAX_HEROINES; i++) {
        if (action->affection_mod[i] != 0) {
            affection_change(i, action->affection_mod[i]);
        }
        if (action->trust_mod[i] != 0) {
            affection_add_trust(i, action->trust_mod[i]);
        }
        if (action->intimacy_mod[i] != 0) {
            affection_add_intimacy(i, action->intimacy_mod[i]);
        }
    }
}

bool gift_give(HeroineID target, GiftID gift) {
    if (target >= MAX_HEROINES || gift >= GIFT_COUNT) return false;
    
    const Gift* g = &g_gifts[gift];
    
    // Check if unique and already given
    if (g->unique && (g_game.heroines[target].flags & (1 << (gift + 16)))) {
        return false; // Already given
    }
    
    // Apply effects
    for (int i = 0; i < MAX_HEROINES; i++) {
        if (g->affection_mod[i] != 0) {
            affection_change(i, g->affection_mod[i]);
        }
    }
    
    // Mark as given
    if (g->unique) {
        g_game.heroines[target].flags |= (1 << (gift + 16));
    }
    
    return true;
}

void event_check_daily(void) {
    // Random daily events
    uint16_t roll = game_rng_next();
    
    if (roll % 100 < 10) { // 10% chance
        // Random heroine event
        HeroineID id = roll % MAX_HEROINES;
        if (affection_check(id, 30)) {
            event_trigger_special(id, 10 + (roll % 5));
        }
    }
}

void event_check_affection_milestones(void) {
    for (int i = 0; i < MAX_HEROINES; i++) {
        Heroine* h = &g_game.heroines[i];
        uint8_t aff = h->affection;
        
        // Confession triggers
        if (aff >= 85 && h->route_progress >= 6 && !(h->flags & 0x20)) {
            h->flags |= 0x20;
            g_game.pending_scene = SCENE_CONFESSION_KIRA + i;
        }
    }
}

bool event_trigger_special(HeroineID id, uint8_t event_id) {
    // Trigger special dialogue/event
    // This would load a specific script segment
    return true;
}

const char* character_get_trait_name(HeroineID id, uint8_t trait) {
    static const char* kira_traits[] = {"Fiery", "Competitive", "Protective", "Vulnerable"};
    static const char* luna_traits[] = {"Analytical", "Detached", "Curious", "Possessive"};
    static const char* rosa_traits[] = {"Gentle", "Shy", "Devoted", "Passionate"};
    
    switch (id) {
        case HEROINE_KIRA: return (trait < 4) ? kira_traits[trait] : "";
        case HEROINE_LUNA: return (trait < 4) ? luna_traits[trait] : "";
        case HEROINE_ROSA: return (trait < 4) ? rosa_traits[trait] : "";
    }
    return "";
}

uint8_t character_get_preferred_gift(HeroineID id) {
    switch (id) {
        case HEROINE_KIRA: return GIFT_WEAPON;
        case HEROINE_LUNA: return GIFT_BOOK;
        case HEROINE_ROSA: return GIFT_FLOWERS;
    }
    return GIFT_FLOWERS;
}

const char* character_get_nickname(HeroineID id, bool intimate) {
    // Intimate nicknames unlocked at high intimacy
    if (!intimate || intimacy_get(id) < 70) {
        return heroine_name(id);
    }
    
    switch (id) {
        case HEROINE_KIRA: return "My Flame";
        case HEROINE_LUNA: return "My Moon";
        case HEROINE_ROSA: return "My Rose";
    }
    return heroine_name(id);
}