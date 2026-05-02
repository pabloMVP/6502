#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>
#include "bus.h"

typedef struct CPU CPU;

typedef enum {
    C = 0x1,
    Z = 0x2,
    I = 0x4,
    D = 0x8,
    B = 0x10,
    UNUSED = 0x20,
    V = 0x40,
    N = 0x80
} STATUS_FLAGS;

typedef enum {
    ACCUMULATOR,
    ABSOLUTE,
    ABSOLUTE_X_INDEXED,
    ABSOLUTE_Y_INDEXED,
    IMMEDIATE,
    IMPLIED,
    INDIRECT,
    X_INDEXED_INDIRECT,
    INDIRECT_Y_INDEXED,
    RELATIVE,
    ZEROPAGE,
    ZEROPAGE_X_INDEXED,
    ZEROPAGE_Y_INDEXED
} ADDRESS_MODE;

typedef void (*InstructionHandler)(CPU *cpu, uint8_t operand);

typedef struct {
    const char *mnemonic;
    InstructionHandler handler;
    ADDRESS_MODE address_mode;
    uint8_t bytes;
} InstructionInfo;

struct CPU {
    uint16_t PC; // program counter
    uint8_t S; // stack pointer
    uint8_t A, X, Y; // registers
    uint8_t P; // processor status flags

    Bus* bus;
};

void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

bool cpu_get_flag(const CPU *cpu, STATUS_FLAGS flag);
void cpu_set_flag(CPU *cpu, STATUS_FLAGS flag, bool set);

uint8_t cpu_fetch(CPU *cpu);
const InstructionInfo *cpu_decode(uint8_t opcode);

// ADC (Add Memory to Accumulator with Carry)
void op_adc(CPU *cpu, uint8_t *operand);
// AND (AND Memory with Accumulator)
void op_and(CPU *cpu, uint8_t *operand); 
// ASL (Shift Left One Bit (Memory or Accumulator))
void op_asl(CPU *cpu, uint8_t *operand);
// BCC (Branch on Carry Clear)
void op_bcc(CPU *cpu, uint8_t *operand);

#endif // CPU_H
