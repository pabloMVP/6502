#include "cpu.h"
#include <stddef.h>

static const InstructionInfo instruction_table[256] = {
    // ADC (Add Memory to Accumulator with Carry)
    [0x69] = { "ADC", op_adc, IMMEDIATE, 2},
    [0x65] = { "ADC", op_adc, ZEROPAGE, 2},
    [0x75] = { "ADC", op_adc, ZEROPAGE_X_INDEXED, 2},
    [0x6D] = { "ADC", op_adc, ABSOLUTE, 3},
    [0x7D] = { "ADC", op_adc, ABSOLUTE_X_INDEXED, 3},
    [0x79] = { "ADC", op_adc, ABSOLUTE_Y_INDEXED, 3},
    [0x61] = { "ADC", op_adc, X_INDEXED_INDIRECT, 2},
    [0x71] = { "ADC", op_adc, INDIRECT_Y_INDEXED, 2},

    // AND (AND Memory with Accumulator)
    [0x29] = { "AND", op_and, IMMEDIATE, 2},
    [0x25] = { "AND", op_and, ZEROPAGE, 2},
    [0x35] = { "AND", op_and, ZEROPAGE_X_INDEXED, 2},
    [0x2D] = { "AND", op_and, ABSOLUTE, 3},
    [0x3D] = { "AND", op_and, ABSOLUTE_X_INDEXED, 3},
    [0x39] = { "AND", op_and, ABSOLUTE_Y_INDEXED, 3},
    [0x21] = { "AND", op_and, X_INDEXED_INDIRECT, 2},
    [0x31] = { "AND", op_and, INDIRECT_Y_INDEXED, 2},

    // ASL Shift Left One Bit (Memory or Accumulator)
    [0x0A] = { "ASL", op_asl, ACCUMULATOR, 1},
    [0x06] = { "ASL", op_asl, ZEROPAGE, 2},
    [0x16] = { "ASL", op_asl, ZEROPAGE_X_INDEXED, 2},
    [0x0E] = { "ASL", op_asl, ABSOLUTE, 3},
    [0x1E] = { "ASL", op_asl, ABSOLUTE_X_INDEXED, 3},
    
    //BCC Branch on Carry Clear
    [0x90] = { "BCC", op_bcc, RELATIVE, 2 },
    
    //BCS Branch on Carry Set
    [0xB0] = { "BCS", op_bcs, RELATIVE, 2 },

    //BEQ Branch on Result Zero
    [0xF0] = { "BEQ", op_beq, RElATIVE, 2 }, 

    //BIT Test Bits in Memory with Accumulator
    [0x24] = { "BIT", op_bit, ZEROPAGE, 2 },
    [0x2C] = { "BIT", op_bit, ABSOLUTE, 3 },

    //BMI Branch on Result Minus
    [0x30] = { "BMI", op_bmi, RELATIVE, 2 },
  
    //BNE Branch on Result Not Zero
    [0xD0] = { "BNE", op_bne, RELATIVE, 2 },

    //BPL Branch on Result Plus
    [0x10] = { "BPL", op_bpl, RELATIVE, 2 }, 

    //BRK Force Break
    [0x00] = { "BRK", op_brk, IMPLIED, 1},

    //BVC Branch on Overflow Clear
    [0x50] = { "BVC", op_bvc, RELATIVE, 2},


    [0xA9] = { "LDA", op_lda, IMMEDIATE, 2 },
    [0xAA] = { "TAX", op_tax, IMPLIED, 1 },
    [0xE8] = { "INX", op_inx, IMPLIED, 1 },
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

void op_not_implemented(CPU *cpu, uint8_t *operand) {
    (void)cpu;
    (void)operand;
}

// ADC implementations

void op_adc(CPU *cpu, uint8_t *operand){
    uint8_t carry = cpu_get_flag(cpu, C);
    
    uint16_t result = *operand + cpu->A + carry;
    
    uint8_t finalResult = (uint8_t)result;

    // Set flags
    cpu_set_flag(cpu, Z, finalResult == 0);
    cpu_set_flag(cpu, N, (finalResult & 0x80) != 0);
    cpu_set_flag(cpu, C, result > 0xFF);
    if ((*operand & 0x80) == (cpu->A & 0x80))
    {
        cpu_set_flag(cpu, V, (*operand & 0x80) != (finalResult & 0x80));
    }
    else
    {
        cpu_set_flag(cpu, V, false);
    }

    // Update accumulator register
    cpu->A = finalResult;
}

void op_and(CPU *cpu, uint8_t *operand){
    cpu->A &= *operand;

    // Set flags
    cpu_set_flag(cpu, Z, cpu->A == 0);
    cpu_set_flag(cpu, N, (cpu->A & 0x80) != 0);
}

void op_asl(CPU *cpu, uint8_t *operand){
    cpu_set_flag(cpu, C, (*operand & 0x80) != 0);
    *operand <<= 1;
    cpu_set_flag(cpu, N, (*operand & 0x80) != 0);
    cpu_set_flag(cpu, Z, *operand == 0);
}

void op_bcc(CPU *cpu, uint8_t *operand){

}
void op 