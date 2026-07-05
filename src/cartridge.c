#include "cartridge.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Cartridge cartridge_loader(uint8_t* data){
    if (is_valid_rom(data)){
        Cartridge cart = {
            .prg_rom_size = data[4] * 16 * 1024,
            .chr_rom_size = data[5] * 8 * 1024,
            .mirroring = data[6] & 0x01,
            .trainer = data[6] & 0x02,
            .mapper = (data[7] & 0xF0) | (data[6] >> 4)
        };
        size_t prg_offset = 16 + (cart.trainer ? 512 : 0);
        size_t chr_offset = prg_offset + cart.prg_rom_size;
        cart.prg_rom = malloc(cart.prg_rom_size);
        memcpy(cart.prg_rom, data + prg_offset, cart.prg_rom_size);
        cart.chr_rom = malloc(cart.chr_rom_size);
        memcpy(cart.chr_rom, data + chr_offset, cart.chr_rom_size);

        return cart;
    }
    else{
        return (Cartridge){0};
    }
}

uint8_t cartridge_prg_read(Cartridge *cart, uint16_t addr){
    return cart->prg_rom[(addr - 0x8000) % cart->prg_rom_size];
}

void cartridge_prg_write(Cartridge *cart, uint16_t addr, uint8_t val){
}

uint8_t cartridge_chr_read(Cartridge *cart, uint16_t addr){
    return cart->chr_rom[addr];
}

void cartridge_chr_write(Cartridge *cart, uint16_t addr, uint8_t val){
}

void cartridge_free(Cartridge *cart){
    free(cart->chr_rom);
    free(cart->prg_rom);
}

bool is_valid_rom(uint8_t* data){
    if (data[0] == 0x4E && data[1] == 0x45 && data[2] == 0x53 && data[3] == 0x1A){
        return true;
    }
    else {
        return false;
    }
}