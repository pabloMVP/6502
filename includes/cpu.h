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

typedef void (*InstructionHandler)(CPU *cpu);

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

void op_not_implemented(CPU *cpu);
void op_lda_immediate(CPU *cpu);
void op_tax_implied(CPU *cpu);
void op_inx_implied(CPU *cpu);
void op_nop_implied(CPU *cpu);

// ADC (Add Memory to Accumulator with Carry)
void op_adc_common(CPU *cpu, uint8_t operand); //common behaviour adc
void op_adc_immediate(CPU *cpu);
void op_adc_zeropage(CPU *cpu);
void op_adc_zeropage_x(CPU *cpu);
void op_adc_absolute(CPU *cpu);
void op_adc_absolute_X(CPU *cpu);
void op_adc_absolute_Y(CPU *cpu);
void op_adc_indirect_X(CPU *cpu);
void op_adc_indirect_Y(CPU *cpu);

// AND (AND Memory with Accumulator)
void op_and_immediate(CPU *cpu);
void op_and_zeropage(CPU *cpu);
void op_and_zeropage_x(CPU *cpu);
void op_and_absolute(CPU *cpu);
void op_and_absolute_X(CPU *cpu);
void op_and_absolute_Y(CPU *cpu);
void op_and_indirect_X(CPU *cpu);
void op_and_indirect_Y(CPU *cpu);

// ASL Shift Left One Bit (Memory or Accumulator)
void op_asl_accumulator(CPU *cpu);
void op_asl_zeropage(CPU *cpu);
void op_asl_zeropage_x(CPU *cpu);
void op_asl_absolute(CPU *cpu);
void op_asl_absolute_X(CPU *cpu);

//BCC Branch on Carry Clear
void op_bcc_relative(CPU *cpu);

//BCS Branch on Carry Set
void op_bcs_relative(CPU *cpu);

//BEQ Branch on Result Zero
void op_beq_relative(CPU *cpu);

//BIT Test Bits in Memory with Accumulator
void op_bit_zeropage(CPU *cpu);
void op_bit_absolute(CPU *cpu);

//BMI Branch on Result Minus
void op_bmi_relative(CPU *cpu);

//BNE Branch on Result Not Zero
void op_bne_relative(CPU *cpu);

//BPL Branch on Result Plus
void op_bpl_relative(CPU *cpu);

//BRK Force Break
void op_brk_implied(CPU *cpu);

//BVC Branch on Overflow Clear
void op_bvc_relative(CPU *cpu);
#endif // CPU_H
