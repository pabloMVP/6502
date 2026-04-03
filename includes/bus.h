#ifndef BUS_H
#define BUS_H
#include <stdint.h>

typedef struct { 
    uint8_t data[65536]; } Bus;

void bus_write(Bus *bus, uint16_t addr, uint8_t val);
uint8_t bus_read(Bus *bus, uint16_t addr);

#endif // BUS_H
