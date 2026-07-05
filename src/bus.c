#include "bus.h"
#include <string.h>

void bus_write(Bus* bus, uint16_t addr, uint8_t val){
    switch (addr)
    {
        case 0x0000 ... 0x1FFF:
            bus->ram[addr & 0x07FF] = val;
            break;
        case 0x2000 ... 0x401F:
            break;
        case 0x4020 ... 0x7FFF:
            break;
        case 0x8000 ... 0xFFFF:
            cartridge_prg_write(bus->cart, addr, val);
            break;
        default:
            break; 
    }
}

uint8_t bus_read(const Bus* bus, uint16_t addr){
    switch (addr)
    {
        case 0x0000 ... 0x1FFF:
            return bus->ram[addr & 0x07FF];
            break;
        case 0x2000 ... 0x401F:
            return 0;
            break;
        case 0x4020 ... 0x7FFF:
            return 0;
            break;
        case 0x8000 ... 0xFFFF:
            return cartridge_prg_read(bus->cart, addr);
            break;
        default:
            return 0;
            break; 
    }
}
