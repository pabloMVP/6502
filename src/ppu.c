#include "ppu.h"


uint8_t ppu_read_register(PPU *ppu, uint16_t reg){
    switch(reg)
    {
        case (PPUSTATUS & 0x0007):
        {
            uint8_t old_status = ppu->PPUSTATUS;
            ppu->w = false;
            ppu->PPUSTATUS &= 0x7F;
            return old_status;
            break;
        }
        case (OAMDATA & 0x0007):
        {
            return 0;
            break;
        }
        case (PPUDATA & 0x0007):
        {
            return 0;
            break;
        }
        default:
            return 0;
            break;
    }
}

void ppu_write_register(PPU *ppu, uint16_t reg, uint8_t val){
    switch(reg)
    {
        case (PPUCTRL & 0x0007):
        {
            ppu->PPUCTRL = val;
            break;
        }
        case (PPUMASK & 0x0007):
        {
            ppu->PPUMASK = val;
            break;
        }
        case (PPUADDR & 0x0007):
        {
            if (!ppu->w){
                ppu->t = (uint16_t)val << 8;
                ppu->w = true; 
            }
            else{
                uint8_t hi_byte = ppu->t & 0xFF00;
                ppu->t = (hi_byte << 8) | (0xFF & val);
                ppu->w = false;
                ppu->t = ppu->v;
            }
            break;
        }
    }
}


uint8_t ppu_chr_read(PPU *ppu, uint16_t addr){
    // PPU address space is 14 bits: 0x0000–0x1FFF
    return cartridge_chr_read(ppu->cartridge, addr);
}

void ppu_render_pattern_table(PPU *ppu, int table, uint8_t *fb){
    // CHR ROM consists of two tables of 4KB each. 
    // The first one goes 0x0000–0x0FFF and second one 0x1000–0x1FFF.
    uint16_t base = table*0x1000; 
    
    // Tiles are a 8x8 pixel grid. Each row is codified by two independent bytes.
    // This is due to each pixel being codified by two bits which 
    // yields 4 possible greys for the pixel. Pixels in a row, from left to right,
    // correspond to the left->right bits in each byte. Here we are scanning 
    // row by row (index r).
    for (uint8_t r = 0; r < 8; r++){
        uint8_t row_A = ppu_chr_read(ppu, base + tile*16 + r);      //First table
        uint8_t row_B = ppu_chr_read(ppu, base + tile*16 + 8 + r);  //Second table, 8 bits offset
        for (uint8_t c = 0; c < 8; c++){
            uint8_t pixel = (((row_A >> (7 - c)) & 0x01) << 1) | ((row_A >> (7-c)) & 0x01);
        }
    }
}