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

typedef struct {
    bool isAccumulator;
    uint16_t loc;
} Operand;

uint8_t operand_read(CPU *cpu, Operand *operand);
void operand_write(CPU *cpu, Operand *operand, uint8_t val);

typedef void (*InstructionHandler)(CPU *cpu, Operand *operand);

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
void cpu_execute(CPU *cpu, ADDRESS_MODE address_mode, InstructionHandler handler);

// ADC (Add Memory to Accumulator with Carry)
void op_adc(CPU *cpu, Operand *operand);
// AND (AND Memory with Accumulator)
void op_and(CPU *cpu, Operand *operand); 
// ASL (Shift Left One Bit (Memory or Accumulator))
void op_asl(CPU *cpu, Operand *operand);
// BCC (Branch on Carry Clear)
void op_bcc(CPU *cpu, Operand *operand);
void op_bcs(CPU *cpu, Operand *operand);
void op_beq(CPU *cpu, Operand *operand);
void op_bit(CPU *cpu, Operand *operand);
void op_bmi(CPU *cpu, Operand *operand);
void op_bne(CPU *cpu, Operand *operand);
void op_bpl(CPU *cpu, Operand *operand);
void op_brk(CPU *cpu, Operand *operand);
void op_bvc(CPU *cpu, Operand *operand);
void op_bvs(CPU *cpu, Operand *operand);
void op_clc(CPU *cpu, Operand *operand);
void op_cld(CPU *cpu, Operand *operand);
void op_cli(CPU *cpu, Operand *operand);
void op_clv(CPU *cpu, Operand *operand);
void op_cmp(CPU *cpu, Operand *operand);
void op_lda(CPU *cpu, Operand *operand);
void op_tax(CPU *cpu, Operand *operand);
void op_inx(CPU *cpu, Operand *operand);
void op_nop_implied(CPU *cpu, Operand *operand);
void op_not_implemented(CPU *cpu, Operand *operand);

void op_and_immediate(CPU *cpu, Operand *operand);

#endif // CPU_H
