// Minimal tonc.h stub for syntax checking
// In production, use actual tonc from devkitPro

#ifndef TONC_STUB_H
#define TONC_STUB_H

#include <stdint.h>
#include <stdbool.h>

// GBA Register addresses
#define REG_BASE 0x04000000
#define REG_DISPCNT (*(volatile uint32_t*)(REG_BASE + 0x00))
#define REG_DISPSTAT (*(volatile uint16_t*)(REG_BASE + 0x04))
#define REG_VCOUNT (*(volatile uint16_t*)(REG_BASE + 0x06))
#define REG_KEYINPUT (*(volatile uint16_t*)(REG_BASE + 0x130))
#define REG_IME (*(volatile uint16_t*)(REG_BASE + 0x208))
#define REG_IE (*(volatile uint16_t*)(REG_BASE + 0x200))
#define REG_IF (*(volatile uint16_t*)(REG_BASE + 0x202))
#define REG_TM0D (*(volatile uint16_t*)(REG_BASE + 0x100))
#define REG_TM0CNT (*(volatile uint16_t*)(REG_BASE + 0x102))

#define REG_BG0CNT (*(volatile uint16_t*)(REG_BASE + 0x08))
#define REG_BG1CNT (*(volatile uint16_t*)(REG_BASE + 0x0A))
#define REG_BG2CNT (*(volatile uint16_t*)(REG_BASE + 0x0C))
#define REG_BG3CNT (*(volatile uint16_t*)(REG_BASE + 0x0E))

#define REG_BG0HOFS (*(volatile uint16_t*)(REG_BASE + 0x10))
#define REG_BG0VOFS (*(volatile uint16_t*)(REG_BASE + 0x12))
#define REG_BG1HOFS (*(volatile uint16_t*)(REG_BASE + 0x14))
#define REG_BG1VOFS (*(volatile uint16_t*)(REG_BASE + 0x16))
#define REG_BG2HOFS (*(volatile uint16_t*)(REG_BASE + 0x18))
#define REG_BG2VOFS (*(volatile uint16_t*)(REG_BASE + 0x1A))
#define REG_BG3HOFS (*(volatile uint16_t*)(REG_BASE + 0x1C))
#define REG_BG3VOFS (*(volatile uint16_t*)(REG_BASE + 0x1E))

#define REG_BLDCNT (*(volatile uint16_t*)(REG_BASE + 0x50))
#define REG_BLDALPHA (*(volatile uint16_t*)(REG_BASE + 0x52))
#define REG_BLDY (*(volatile uint16_t*)(REG_BASE + 0x54))

// Display control bits
#define DCNT_MODE0 0x0000
#define DCNT_MODE1 0x0001
#define DCNT_MODE2 0x0002
#define DCNT_MODE3 0x0003
#define DCNT_BG0 0x0100
#define DCNT_BG1 0x0200
#define DCNT_BG2 0x0400
#define DCNT_BG3 0x0800
#define DCNT_OBJ 0x1000
#define DCNT_OBJ_1D 0x0040
#define DCNT_BLANK 0x0080

// BG control
#define BG_CBB(bb) ((bb) << 2)
#define BG_SBB(bb) ((bb) << 8)
#define BG_REG_32x32 0x0000
#define BG_REG_64x32 0x4000
#define BG_REG_32x64 0x8000
#define BG_REG_64x64 0xC000
#define BG_PRIO(p) (p)
#define BG_MOSAIC 0x0040
#define BG_WRAP 0x2000

// Interrupts
#define II_VBLANK 0x0001
#define II_HBLANK 0x0002
#define II_VCOUNT 0x0004
#define II_TM0 0x0008
#define II_TM1 0x0010
#define II_TM2 0x0020
#define II_TM3 0x0040
#define II_SERIAL 0x0080
#define II_DMA0 0x0100
#define II_DMA1 0x0200
#define II_DMA2 0x0400
#define II_DMA3 0x0800
#define II_KEYPAD 0x1000
#define II_GAMEPAK 0x2000

// Keys
#define KEY_ANY 0x03FF
#define KEY_A 0x0001
#define KEY_B 0x0002
#define KEY_SELECT 0x0004
#define KEY_START 0x0008
#define KEY_RIGHT 0x0010
#define KEY_LEFT 0x0020
#define KEY_UP 0x0040
#define KEY_DOWN 0x0080
#define KEY_R 0x0100
#define KEY_L 0x0200

// VRAM
#define VRAM_BASE 0x06000000
#define BG_TILE_BASE_ADR(cbb) (VRAM_BASE + (cbb) * 0x4000)
#define BG_MAP_BASE_ADR(sbb) (VRAM_BASE + (sbb) * 0x800)

// Palette
#define BG_PALETTE ((volatile uint16_t*)0x05000000)
#define OBJ_PALETTE ((volatile uint16_t*)0x05000200)

#define RGB15(r,g,b) ((r) | ((g)<<5) | ((b)<<10))

// Blending
#define BLD_BUILD(top, bot) ((top) | ((bot)<<8))
#define BLD_BG0 0x0001
#define BLD_BG1 0x0002
#define BLD_BG2 0x0004
#define BLD_BG3 0x0008
#define BLD_OBJ 0x0010
#define BLD_WHITE 0x0040
#define BLD_STD 0x0000
#define BLD_ALPHA 0x0040

// DMA
#define REG_DMA0SAD (*(volatile uint32_t*)(REG_BASE + 0x0B0))
#define REG_DMA0DAD (*(volatile uint32_t*)(REG_BASE + 0x0B4))
#define REG_DMA0CNT (*(volatile uint32_t*)(REG_BASE + 0x0B8))
#define REG_DMA1SAD (*(volatile uint32_t*)(REG_BASE + 0x0BC))
#define REG_DMA1DAD (*(volatile uint32_t*)(REG_BASE + 0x0C0))
#define REG_DMA1CNT (*(volatile uint32_t*)(REG_BASE + 0x0C4))
#define REG_DMA2SAD (*(volatile uint32_t*)(REG_BASE + 0x0C8))
#define REG_DMA2DAD (*(volatile uint32_t*)(REG_BASE + 0x0CC))
#define REG_DMA2CNT (*(volatile uint32_t*)(REG_BASE + 0x0D0))
#define REG_DMA3SAD (*(volatile uint32_t*)(REG_BASE + 0x0D4))
#define REG_DMA3DAD (*(volatile uint32_t*)(REG_BASE + 0x0D8))
#define REG_DMA3CNT (*(volatile uint32_t*)(REG_BASE + 0x0DC))

#define DMA_ENABLE 0x80000000
#define DMA_TIMING_IMMEDIATE 0x00000000
#define DMA_TIMING_VBLANK 0x20000000
#define DMA_TIMING_HBLANK 0x40000000
#define DMA_32 0x04000000
#define DMA_16 0x00000000
#define DMA_REPEAT 0x02000000
#define DMA_SRC_INC 0x00000000
#define DMA_SRC_DEC 0x00800000
#define DMA_SRC_FIXED 0x01000000
#define DMA_DST_INC 0x00000000
#define DMA_DST_DEC 0x00200000
#define DMA_DST_FIXED 0x00400000
#define DMA_DST_RELOAD 0x00600000

// OAM
#define OAM_BASE 0x07000000
#define OBJ_ATTR0 0
#define OBJ_ATTR1 1
#define OBJ_ATTR2 2

#define ATTR0_SQUARE 0x0000
#define ATTR0_WIDE 0x4000
#define ATTR0_TALL 0x8000
#define ATTR0_8BPP 0x2000
#define ATTR0_MOSAIC 0x1000
#define ATTR0_BLEND 0x0800
#define ATTR0_WIN0 0x0400
#define ATTR0_WIN1 0x0200
#define ATTR0_WIN_OUT 0x0600

#define ATTR1_SIZE_8 0x0000
#define ATTR1_SIZE_16 0x4000
#define ATTR1_SIZE_32 0x8000
#define ATTR1_SIZE_64 0xC000
#define ATTR1_HFLIP 0x1000
#define ATTR1_VFLIP 0x2000

#define ATTR2_PRIO(p) (p)
#define ATTR2_PALETTE(p) ((p) << 12)

// Common types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

// BIOS calls (stubs - these are functions in real tonc)
extern void VBlankIntrWait(void);
extern void IntrWait(void);
extern void irq_init(void* handler);
extern void irq_add(int id, void* handler);

#define VBlankIntrWait() do {} while(0)
#define IntrWait() do {} while(0)
#define irq_init(x) do {} while(0)
#define irq_add(a,b) do {} while(0)

#endif // TONC_STUB_H