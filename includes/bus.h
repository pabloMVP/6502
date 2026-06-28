#ifndef BUS_H
#define BUS_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct { 
    uint8_t data[65536]; 
} Bus;

void bus_write(Bus *bus, uint16_t addr, uint8_t val);
uint8_t bus_read(const Bus *bus, uint16_t addr);
bool bus_load(Bus *bus, uint16_t addr, const uint8_t *data, size_t len);

#endif // BUS_H
