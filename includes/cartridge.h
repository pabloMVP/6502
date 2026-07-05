#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint8_t* prg_rom;
    uint8_t* chr_rom;
    size_t prg_rom_size;
    size_t chr_rom_size;
    uint8_t mapper;
    bool mirroring;
    bool trainer;
} Cartridge;

Cartridge cartridge_loader(uint8_t* data);
void cartridge_free(Cartridge *cart);

uint8_t cartridge_prg_read(Cartridge *cart, uint16_t addr);
void cartridge_prg_write(Cartridge *cart, uint16_t addr, uint8_t val);

uint8_t cartridge_chr_read(Cartridge *cart, uint16_t addr);
void cartridge_chr_write(Cartridge *cart, uint16_t addr, uint8_t val);
bool is_valid_rom(uint8_t* data);
#endif // CARTRIDGE_H