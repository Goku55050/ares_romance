#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "game.h"

// Character initialization
void characters_init(void);

// Affection system
void affection_change(HeroineID id, int8_t amount);
void affection_add_trust(HeroineID id, uint8_t amount);
void affection_add_intimacy(HeroineID id, uint8_t amount);
bool affection_check(HeroineID id, uint8_t threshold);
uint8_t affection_get(HeroineID id);
uint8_t trust_get(HeroineID id);
uint8_t intimacy_get(HeroineID id);

// Route management
void route_progress(HeroineID id);
bool route_is_locked(HeroineID id);
void route_lock_others(HeroineID chosen);
HeroineID route_get_locked(void);
bool route_all_maxed(void);

// Daily interaction
typedef enum {
    ACTION_TALK,
    ACTION_GIFT,
    ACTION_DATE,
    ACTION_TRAIN,
    ACTION_STUDY,
    ACTION_REST,
    ACTION_SPECIAL
} ActionType;

typedef struct {
    ActionType type;
    HeroineID target;
    uint8_t energy_cost;
    int8_t affection_mod[MAX_HEROINES];
    int8_t trust_mod[MAX_HEROINES];
    int8_t intimacy_mod[MAX_HEROINES];
    uint8_t stamina_gain;
    uint8_t charm_gain;
    uint8_t perception_gain;
    const char* description;
    bool unlocks_scene;
    SceneID scene_id;
} Action;

// Available actions per day
extern const Action g_actions[];
extern const uint8_t g_action_count;

// Perform action
bool action_perform(uint8_t action_index);
void action_apply_effects(const Action* action);

// Gift system
typedef enum {
    GIFT_FLOWERS,
    GIFT_SWEETS,
    GIFT_BOOK,
    GIFT_WEAPON,
    GIFT_ACCESSORY,
    GIFT_SPECIAL_KIRA,
    GIFT_SPECIAL_LUNA,
    GIFT_SPECIAL_ROSA,
    GIFT_COUNT
} GiftID;

typedef struct {
    GiftID id;
    char name[24];
    char description[64];
    int8_t affection_mod[MAX_HEROINES];
    uint8_t cost;
    bool unique;
} Gift;

extern const Gift g_gifts[];
bool gift_give(HeroineID target, GiftID gift);

// Special events
void event_check_daily(void);
void event_check_affection_milestones(void);
bool event_trigger_special(HeroineID id, uint8_t event_id);

// Character-specific data
const char* character_get_trait_name(HeroineID id, uint8_t trait);
uint8_t character_get_preferred_gift(HeroineID id);
const char* character_get_nickname(HeroineID id, bool intimate);

#endif // CHARACTERS_H