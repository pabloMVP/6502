#include "cpu.h"
#include <stddef.h>

static const InstructionInfo instruction_table[256] = {
    // ADC (Add Memory to Accumulator with Carry)
    [0x69] = { "ADC", op_adc_immediate, IMMEDIATE, 2},
    [0x65] = { "ADC", op_adc_zeropage, ZEROPAGE, 2},
    [0x75] = { "ADC", op_adc_zeropage_x, ZEROPAGE_X_INDEXED, 2},
    [0x6D] = { "ADC", op_adc_absolute, ABSOLUTE, 3},
    [0x7D] = { "ADC", op_adc_absolute_X, ABSOLUTE_X_INDEXED, 3},
    [0x79] = { "ADC", op_adc_absolute_Y, ABSOLUTE_Y_INDEXED, 3},
    [0x61] = { "ADC", op_adc_indirect_X, X_INDEXED_INDIRECT, 2},
    [0x71] = { "ADC", op_adc_indirect_Y, INDIRECT_Y_INDEXED, 2},

    // AND (AND Memory with Accumulator)
    [0x29] = { "AND", op_and_immediate, IMMEDIATE, 2},
    [0x25] = { "AND", op_and_zeropage, ZEROPAGE, 2},
    [0x35] = { "AND", op_and_zeropage_x, ZEROPAGE_X_INDEXED, 2},
    [0x2D] = { "AND", op_and_absolute, ABSOLUTE, 3},
    [0x3D] = { "AND", op_and_absolute_X, ABSOLUTE_X_INDEXED, 3},
    [0x39] = { "AND", op_and_absolute_Y, ABSOLUTE_Y_INDEXED, 3},
    [0x21] = { "AND", op_and_indirect_X, X_INDEXED_INDIRECT, 2},
    [0x31] = { "AND", op_and_indirect_Y, INDIRECT_Y_INDEXED, 2},

    // ASL Shift Left One Bit (Memory or Accumulator)
    [0x0A] = { "ASL", op_asl_accumulator, ACCUMULATOR, 1},
    [0x06] = { "ASL", op_asl_zeropage, ZEROPAGE, 2},
    [0x16] = { "ASL", op_asl_zeropage_x, ZEROPAGE_X_INDEXED, 2},
    [0x0E] = { "ASL", op_asl_absolute, ABSOLUTE, 3},
    [0x1E] = { "ASL", op_asl_absolute_X, ABSOLUTE_X_INDEXED, 3},
    
    [0xA9] = { "LDA", op_lda_immediate, IMMEDIATE, 2 },
    [0xAA] = { "TAX", op_tax_implied, IMPLIED, 1 },
    [0xE8] = { "INX", op_inx_implied, IMPLIED, 1 },
    [0xEA] = { "NOP", op_nop_implied, IMPLIED, 1 },

};

static const InstructionInfo not_implemented = {
    "???",
    op_not_implemented,
    IMPLIED,
    1
};

const InstructionInfo *cpu_decode(uint8_t opcode) {
    if (instruction_table[opcode].handler == NULL) {
        return &not_implemented;
    }

    return &instruction_table[opcode];
}

void op_not_implemented(CPU *cpu) {
    (void)cpu;
}

void op_lda_immediate(CPU *cpu) {
    uint8_t value = cpu_fetch(cpu);
    cpu->A = value;
}

void op_tax_implied(CPU *cpu) {
    cpu->X = cpu->A;
}

void op_inx_implied(CPU *cpu) {
    cpu->X += 1;
}

void op_nop_implied(CPU *cpu) {
    (void)cpu;
}
