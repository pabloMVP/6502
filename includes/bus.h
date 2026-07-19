#ifndef BUS_H
#define BUS_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cartridge.h"
#include "ppu.h"

typedef struct { 
    Cartridge* cart; 
    PPU* ppu;
    uint8_t ram[2048]; 
} Bus;

void bus_write(Bus *bus, uint16_t addr, uint8_t val);
uint8_t bus_read(const Bus *bus, uint16_t addr);

#endif // BUS_H
