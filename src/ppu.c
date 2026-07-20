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
            if ((ppu->v > 0x3F00) && (ppu->v < 0x3FFF)){
                ppu->read_buffer = ppu->v;
                return ppu_mem_read(ppu, ppu->v);
            }
            uint8_t buffer_old = ppu->read_buffer;
            ppu->read_buffer = ppu_mem_read(ppu, ppu->v);
            if (ppu->PPUCTRL & 0x04){
                ppu->v += 32;
            }
            else{
                ppu->v += 1;
            }
            return buffer_old;
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
                ppu->t = (ppu->t & 0x00FF) | (val << 8);
                ppu->w = true; 
            }
            else{
                ppu->t = (ppu->t & 0xFF00) |  val;
                ppu->w = false;
                ppu->v = ppu->t;
                ppu->v &= 0x3FFF;
            }
            break;
        }
        case (PPUDATA & 0x0007):
        {
            ppu_mem_write(ppu, ppu->v, val);
            if (ppu->PPUCTRL & 0x04){
                ppu->v += 32;
            }
            else{
                ppu->v += 1;
            }
            break;
        }
    }
}


uint8_t ppu_chr_read(PPU *ppu, uint16_t addr){
    // PPU address space is 14 bits: 0x0000–0x1FFF
    return cartridge_chr_read(ppu->cartridge, addr);
}

void ppu_mem_write(PPU *ppu, uint16_t addr, uint8_t val){
    switch(addr){
        case 0x0000 ... 0x1FFF:
        {
            cartridge_chr_write(ppu->cartridge, addr, val);
            break;
        }
        case 0x2000 ... 0x3EFF:
        {
            ppu->vram[(addr - 0x2000) & 0x07FF] = val;
            break;
        }
        case 0x3F00 ... 0x3FFF:
        {
            ppu->palettes_ram[(addr - 0x3F00) & (0x1F)] = val;
            break;
        }
        default:
            break;
    }
}

uint8_t ppu_mem_read(PPU *ppu, uint16_t addr){
    switch(addr){
        case 0x0000 ... 0x1FFF:
        {
            return cartridge_chr_read(ppu->cartridge, addr);
            break;
        }
        case 0x2000 ... 0x3EFF:
        {
            return ppu->vram[(addr - 0x2000) & 0x07FF];
            break;
        }
        case 0x3F00 ... 0x3FFF:
        {
            return ppu->palettes_ram[(addr - 0x3F00) & (0x1F)];
            break;
        }
        default:
        {
            return 0;
            break;
        }
    }
}

// void ppu_render_pattern_table(PPU *ppu, int table, uint8_t *fb){
//     // CHR ROM consists of two tables of 4KB each. 
//     // The first one goes 0x0000–0x0FFF and second one 0x1000–0x1FFF.
//     uint16_t base = table*0x1000; 
    
//     // Tiles are a 8x8 pixel grid. Each row is codified by two independent bytes.
//     // This is due to each pixel being codified by two bits which 
//     // yields 4 possible greys for the pixel. Pixels in a row, from left to right,
//     // correspond to the left->right bits in each byte. Here we are scanning 
//     // row by row (index r).
//     for (uint8_t r = 0; r < 8; r++){
//         uint8_t row_A = ppu_chr_read(ppu, base + tile*16 + r);      //First table
//         uint8_t row_B = ppu_chr_read(ppu, base + tile*16 + 8 + r);  //Second table, 8 bits offset
//         for (uint8_t c = 0; c < 8; c++){
//             uint8_t pixel = (((row_A >> (7 - c)) & 0x01) << 1) | ((row_A >> (7-c)) & 0x01);
//         }
//     }
// }