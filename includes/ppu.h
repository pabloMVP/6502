#ifndef PPU_H
#define PPU_H
#include "cartridge.h"
typedef struct {
    Cartridge* cartridge;
} PPU;

uint8_t ppu_chr_read(PPU *ppu, int tile, uint16_t addr);
void ppu_render_pattern_table(PPU *ppu, int table, uint8_t *fb);  // table 0 o 1

#endif // PPU_H