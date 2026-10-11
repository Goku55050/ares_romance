#ifndef SAVE_H
#define SAVE_H

#include "game.h"
#include <stdbool.h>

// SRAM addresses (GBA cartridge SRAM typically at 0x0E000000)
#define SRAM_BASE 0x0E000000
#define SRAM_SIZE 0x10000 // 64KB typical
#define SAVE_SLOT_SIZE sizeof(SaveData)
#define SAVE_BASE_OFFSET 0x1000 // Leave space for header

// Save functions
void save_init(void);
bool save_slot_exists(uint8_t slot);
bool save_game(uint8_t slot);
bool load_game(uint8_t slot);
void save_autosave(void);
void save_delete(uint8_t slot);
uint16_t save_calculate_checksum(const SaveData* data);
bool save_verify_checksum(const SaveData* data);
void save_get_slot_info(uint8_t slot, char* name_out, uint32_t* playtime, uint8_t* chapter);

// Quick save/load (for suspend/resume)
void save_quicksave(void);
bool save_quickload(void);

#endif // SAVE_H