#include "ppu.h"

uint8_t ppu_chr_read(PPU *ppu, uint16_t addr){
    // PPU address space is 14 bits: 0x0000–0x1FFF
    return cartridge_chr_read(ppu->cartridge, addr);
}

void ppu_render_pattern_table(PPU *ppu, int tile, int table, uint8_t *fb){
    // CHR ROM consists of two tables of 4KB each. 
    // The first one goes 0x0000–0x0FFF and second one 0x1000–0x1FFF.
    uint16_t base = table*0x1000; 
    
    // Tiles are a 8x8 pixel grid. Each row is codified by two bytes.
    // This is due to each pixel being codified by two bits which 
    // yields 4 possible greys for the pixel. Pixels in a row, from left to right,
    // correspond to the left->right bits in each byte. Here we are scanning 
    // row by row (index r).
    for (uint8_t r = 0; r < 8; r++){
        uint8_t row_A = ppu_chr_read(ppu, base + tile*16 + r);
        uint8_t row_B = ppu_chr_read(ppu, base + tile*16 + 8 + r);
        for (uint8_t c = 0; c < 8; c++){
            uint8_t pixel = (((row_A >> (7 - c)) & 0x01) << 1) | ((row_A >> (7-c)) & 0x01);
        }
    }
}