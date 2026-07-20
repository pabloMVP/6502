#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include <stdbool.h>
#include "cartridge.h"
typedef struct {
    Cartridge* cartridge;
    uint8_t framebuffer[256* 240];
    uint8_t vram[2048];
    uint8_t palettes_ram[32];
    uint8_t oam[256];
    uint8_t PPUCTRL;
    uint8_t PPUMASK;
    uint8_t PPUSTATUS;
    uint8_t OAMADDR;
    uint16_t v;
    uint16_t t;
    uint8_t x;
    bool w;
    uint8_t read_buffer;
} PPU;

typedef enum {
    PPUCTRL = 0x2000,
    PPUMASK = 0x2001,
    PPUSTATUS = 0x2002,
    OAMADDR = 0x2003,
    OAMDATA = 0x2004,
    PPUSCROLL = 0x2005,
    PPUADDR = 0x2006,
    PPUDATA = 0x2007,
    OAMDMA = 0x4014
} PPU_REGISTERS;

uint8_t ppu_read_register(PPU *ppu, uint16_t reg);
void ppu_write_register(PPU *ppu, uint16_t reg, uint8_t val);

uint8_t ppu_chr_read(PPU *ppu, uint16_t addr);
void ppu_render_pattern_table(PPU *ppu, int table, uint8_t *fb);  // table 0 o 1

void ppu_mem_write(PPU *ppu, uint16_t addr, uint8_t val);
uint8_t ppu_mem_read(PPU *ppu, uint16_t addr);

#endif // PPU_H