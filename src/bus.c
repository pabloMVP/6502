#include "bus.h"
#include <string.h>

void bus_write(Bus* bus, uint16_t addr, uint8_t val){
    bus->data[addr] = val;
}

uint8_t bus_read(const Bus* bus, uint16_t addr){
    return bus->data[addr];
}

bool bus_load(Bus *bus, uint16_t addr, const uint8_t *data, size_t len){
    if ((addr + len) <= 65536){
        memcpy(bus->data + addr, data, len);
        return true;
    }
    else{
        return false;
    }
}