#include "bus.h"

void bus_write(Bus* bus, uint16_t addr, uint8_t val){
    bus->data[addr] = val;
}

uint8_t bus_read(const Bus* bus, uint16_t addr){
    return bus->data[addr];
}
