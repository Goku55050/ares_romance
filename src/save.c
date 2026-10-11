#include "save.h"
#include "game.h"
#include "characters.h"
#include "tonc_stub.h"
#include <string.h>
#include <stdio.h>

// SRAM pointer
static volatile uint8_t* sram = (volatile uint8_t*)SRAM_BASE;

// SRAM wait states (required for reliable access)
#define SRAM_WAIT() { for(volatile int i=0; i<4; i++); }

void save_init(void) {
    // Enable SRAM (if needed - usually automatic on flash carts)
    // Check for valid signature in each slot
    for (int i = 0; i < SAVE_SLOTS; i++) {
        SaveData* slot = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + i * SAVE_SLOT_SIZE);
        SRAM_WAIT();
        if (slot->signature[0] != 'A' || slot->signature[1] != 'R' || 
            slot->signature[2] != 'S' || slot->signature[3] != 'V') {
            // Invalid slot, clear it
            memset((void*)slot, 0, SAVE_SLOT_SIZE);
        }
    }
}

bool save_slot_exists(uint8_t slot) {
    if (slot >= SAVE_SLOTS) return false;
    
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + slot * SAVE_SLOT_SIZE);
    SRAM_WAIT();
    
    return (data->signature[0] == 'A' && data->signature[1] == 'R' &&
            data->signature[2] == 'S' && data->signature[3] == 'V' &&
            save_verify_checksum(data));
}

bool save_game(uint8_t slot) {
    if (slot >= SAVE_SLOTS) return false;
    
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + slot * SAVE_SLOT_SIZE);
    
    // Prepare save data
    SaveData save = {0};
    save.signature[0] = 'A';
    save.signature[1] = 'R';
    save.signature[2] = 'S';
    save.signature[3] = 'V';
    save.version = 1;
    save.playtime_frames = g_game.frame_count;
    save.slot_index = slot;
    save.state = g_game.state;
    save.mc = g_game.mc;
    memcpy(save.heroines, g_game.heroines, sizeof(Heroine) * MAX_HEROINES);
    save.current_line = g_game.current_line;
    save.last_scene = g_game.pending_scene;
    memcpy(save.seen_cgs, g_game.heroines[0].flags & 0xFF00 ? (uint8_t[32]){0} : (uint8_t[32]){0}, 32); // Placeholder
    save.endings_unlocked = 0; // Calculate from flags
    
    // Calculate endings unlocked
    for (int i = 0; i < MAX_HEROINES; i++) {
        if (g_game.heroines[i].flags & 0x10) { // Maxed
            save.endings_unlocked |= (1 << i);
        }
    }
    if (route_all_maxed()) {
        save.endings_unlocked |= (1 << 3); // Harem ending
    }
    
    save.checksum = save_calculate_checksum(&save);
    
    // Write to SRAM (byte by byte for reliability)
    volatile uint8_t* dest = (volatile uint8_t*)data;
    uint8_t* src = (uint8_t*)&save;
    
    for (size_t i = 0; i < SAVE_SLOT_SIZE; i++) {
        dest[i] = src[i];
        SRAM_WAIT();
    }
    
    // Verify
    SRAM_WAIT();
    return save_verify_checksum(data);
}

bool load_game(uint8_t slot) {
    if (slot >= SAVE_SLOTS) return false;
    if (!save_slot_exists(slot)) return false;
    
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + slot * SAVE_SLOT_SIZE);
    SRAM_WAIT();
    
    // Copy from SRAM
    SaveData save;
    volatile uint8_t* src = (volatile uint8_t*)data;
    uint8_t* dest = (uint8_t*)&save;
    
    for (size_t i = 0; i < SAVE_SLOT_SIZE; i++) {
        dest[i] = src[i];
        SRAM_WAIT();
    }
    
    // Verify checksum
    if (!save_verify_checksum(&save)) return false;
    
    // Restore game state
    g_game.frame_count = save.playtime_frames;
    g_game.state = save.state;
    g_game.mc = save.mc;
    memcpy(g_game.heroines, save.heroines, sizeof(Heroine) * MAX_HEROINES);
    g_game.current_line = save.current_line;
    g_game.pending_scene = save.last_scene;
    
    // Load appropriate chapter script
    script_load_chapter(g_game.mc.chapter);
    
    return true;
}

void save_autosave(void) {
    // Auto-save to slot 0
    save_game(0);
}

void save_delete(uint8_t slot) {
    if (slot >= SAVE_SLOTS) return;
    
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + slot * SAVE_SLOT_SIZE);
    volatile uint8_t* dest = (volatile uint8_t*)data;
    
    for (size_t i = 0; i < SAVE_SLOT_SIZE; i++) {
        dest[i] = 0;
        SRAM_WAIT();
    }
}

uint16_t save_calculate_checksum(const SaveData* data) {
    uint16_t checksum = 0;
    const uint8_t* bytes = (const uint8_t*)data;
    
    // Skip checksum field itself
    size_t checksum_offset = offsetof(SaveData, checksum);
    
    for (size_t i = 0; i < SAVE_SLOT_SIZE; i++) {
        if (i >= checksum_offset && i < checksum_offset + 2) continue;
        checksum += bytes[i];
        checksum = (checksum << 1) | (checksum >> 15); // Rotate
    }
    
    return checksum;
}

bool save_verify_checksum(const SaveData* data) {
    return data->checksum == save_calculate_checksum(data);
}

void save_get_slot_info(uint8_t slot, char* name_out, uint32_t* playtime, uint8_t* chapter) {
    if (slot >= SAVE_SLOTS || !save_slot_exists(slot)) {
        if (name_out) strcpy(name_out, "EMPTY");
        if (playtime) *playtime = 0;
        if (chapter) *chapter = 0;
        return;
    }
    
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + slot * SAVE_SLOT_SIZE);
    SRAM_WAIT();
    
    // Read minimal info
    volatile uint8_t* src = (volatile uint8_t*)data;
    uint32_t frames = 0;
    uint8_t ch = 0;
    
    // playtime_frames at offset 4
    frames = src[4] | (src[5] << 8) | (src[6] << 16) | (src[7] << 24);
    // chapter at offset of mc + mc.chapter offset
    // mc starts at offset 8 (sig+ver+frames+slot+state) + MainChar offset
    ch = src[20]; // Approximate
    
    if (name_out) {
        HeroineID locked = route_get_locked();
        if (locked < MAX_HEROINES) {
            sprintf(name_out, "%s Route", heroine_name(locked));
        } else {
            strcpy(name_out, "Common Route");
        }
    }
    if (playtime) *playtime = frames / 60; // Convert to seconds
    if (chapter) *chapter = ch;
}

void save_quicksave(void) {
    // Quick save to last slot (slot 2)
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + 2 * SAVE_SLOT_SIZE);
    
    SaveData save = {0};
    save.signature[0] = 'Q';
    save.signature[1] = 'S';
    save.signature[2] = 'A';
    save.signature[3] = 'V';
    save.version = 1;
    save.playtime_frames = g_game.frame_count;
    save.slot_index = 2;
    save.state = g_game.state;
    save.mc = g_game.mc;
    memcpy(save.heroines, g_game.heroines, sizeof(Heroine) * MAX_HEROINES);
    save.current_line = g_game.current_line;
    save.last_scene = g_game.pending_scene;
    save.checksum = save_calculate_checksum(&save);
    
    volatile uint8_t* dest = (volatile uint8_t*)data;
    uint8_t* src = (uint8_t*)&save;
    
    for (size_t i = 0; i < SAVE_SLOT_SIZE; i++) {
        dest[i] = src[i];
        SRAM_WAIT();
    }
}

bool save_quickload(void) {
    SaveData* data = (SaveData*)(SRAM_BASE + SAVE_BASE_OFFSET + 2 * SAVE_SLOT_SIZE);
    SRAM_WAIT();
    
    // Check quicksave signature
    if (data->signature[0] != 'Q' || data->signature[1] != 'S' ||
        data->signature[2] != 'A' || data->signature[3] != 'V') {
        return false;
    }
    
    if (!save_verify_checksum(data)) return false;
    
    // Restore
    SaveData save;
    volatile uint8_t* src = (volatile uint8_t*)data;
    uint8_t* dest = (uint8_t*)&save;
    
    for (size_t i = 0; i < SAVE_SLOT_SIZE; i++) {
        dest[i] = src[i];
        SRAM_WAIT();
    }
    
    g_game.frame_count = save.playtime_frames;
    g_game.state = save.state;
    g_game.mc = save.mc;
    memcpy(g_game.heroines, save.heroines, sizeof(Heroine) * MAX_HEROINES);
    g_game.current_line = save.current_line;
    g_game.pending_scene = save.last_scene;
    
    script_load_chapter(g_game.mc.chapter);
    
    return true;
}