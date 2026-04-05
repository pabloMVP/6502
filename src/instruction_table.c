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
    
    //BCC Branch on Carry Clear
    [0x90] = { "BCC", op_bcc_relative, RELATIVE, 2 },
    
    //BCS Branch on Carry Set
    [0xB0] = { "BCS", op_bcs_relative, RELATIVE, 2 },

    //BEQ Branch on Result Zero
    [0xF0] = { "BEQ", op_beq_relative, RELATIVE, 2 }, 

    //BIT Test Bits in Memory with Accumulator
    [0x24] = { "BIT", op_bit_zeropage, ZEROPAGE, 2 },
    [0x2C] = { "BIT", op_bit_absolute, ABSOLUTE, 3 },

    //BMI Branch on Result Minus
    [0x30] = { "BMI", op_bmi_relative, RELATIVE, 2 },
  
    //BNE Branch on Result Not Zero
    [0xD0] = { "BNE", op_bne_relative, RELATIVE, 2 },

    //BPL Branch on Result Plus
    [0x10] = { "BPL", op_bpl_relative, RELATIVE, 2 }, 

    //BRK Force Break
    [0x00] = { "BRK", op_brk_implied, IMPLIED, 1},

    //BVC Branch on Overflow Clear
    [0x50] = { "BVC", op_bvc_relative, RELATIVE, 2},


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

// ADC implementations

void op_adc_common(CPU *cpu, uint8_t operand){
    uint8_t carry = cpu_get_flag(cpu, C);
    
    uint16_t result = operand + cpu->A + carry;
    
    uint8_t finalResult = (uint8_t)result;

    // Set flags
    cpu_set_flag(cpu, Z, finalResult == 0);
    cpu_set_flag(cpu, N, (finalResult & 0x80) != 0);
    cpu_set_flag(cpu, C, result > 0xFF);
    if ((operand & 0x80) == (cpu->A & 0x80))
    {
        cpu_set_flag(cpu, V, (operand & 0x80) != (finalResult & 0x80));
    }
    else
    {
        cpu_set_flag(cpu, V, false);
    }

    // Update accumulator register
    cpu->A = finalResult;
}

void op_adc_immediate(CPU *cpu) {
    uint8_t operand = cpu_fetch(cpu);
    op_adc_common(cpu, operand);
}


void op_adc_zeropage(CPU *cpu) {
    uint8_t address = cpu_fetch(cpu);
    uint8_t operand = bus_read(cpu->bus, address);
    op_adc_common(cpu, operand);
}

void op_adc_zeropage_x(CPU *cpu) {
    uint8_t address = cpu_fetch(cpu);
    address += cpu->X;
    uint8_t operand = bus_read(cpu->bus, address);
    op_adc_common(cpu, operand);
}

void op_adc_absolute(CPU *cpu) {
    uint8_t address_LL = cpu_fetch(cpu);
    uint8_t address_HH = cpu_fetch(cpu);
    uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
    uint8_t operand = bus_read(cpu->bus, address);
    op_adc_common(cpu, operand);
}

void op_adc_absolute_X(CPU *cpu) {
    uint8_t address_LL = cpu_fetch(cpu);
    uint8_t address_HH = cpu_fetch(cpu);
    uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
    uint8_t operand = bus_read(cpu->bus, address + cpu->X);
    op_adc_common(cpu, operand);
}

void op_adc_absolute_Y(CPU *cpu) {
    uint8_t address_LL = cpu_fetch(cpu);
    uint8_t address_HH = cpu_fetch(cpu);
    uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
    uint8_t operand = bus_read(cpu->bus, address + cpu->Y);
    op_adc_common(cpu, operand);
}

void op_adc_indirect_X(CPU *cpu) {
    uint8_t indirect_address_LL = cpu_fetch(cpu);
    uint8_t final_address_LL = bus_read(cpu->bus, (indirect_address_LL + cpu->X) & 0xFF); 
    uint8_t final_address_HH = bus_read(cpu->bus, (indirect_address_LL + cpu->X + 1) & 0xFF); 
    uint8_t operand = bus_read(cpu->bus, (uint16_t)(final_address_HH << 8) | final_address_LL);
    op_adc_common(cpu, operand);
}

void op_adc_indirect_Y(CPU *cpu) {
    uint8_t indirect_address_LL = cpu_fetch(cpu);
    uint8_t final_address_LL = bus_read(cpu->bus, indirect_address_LL & 0xFF); 
    uint8_t final_address_HH = bus_read(cpu->bus, (indirect_address_LL + 1) & 0xFF); 
    uint8_t operand = bus_read(cpu->bus, ((uint16_t)(final_address_HH << 8) | final_address_LL) + cpu->Y);
    op_adc_common(cpu, operand);
}