#include "cpu.h"
#include <stddef.h>
#include <stdio.h>

static const InstructionInfo instruction_table[256] = {
    // ADC (Add Memory to Accumulator with Carry)
    [0x69] = { "ADC", op_adc, IMMEDIATE, 2, 2},
    [0x65] = { "ADC", op_adc, ZEROPAGE, 2, 3},
    [0x75] = { "ADC", op_adc, ZEROPAGE_X_INDEXED, 2, 4},
    [0x6D] = { "ADC", op_adc, ABSOLUTE, 3, 4},
    [0x7D] = { "ADC", op_adc, ABSOLUTE_X_INDEXED, 3, 4},
    [0x79] = { "ADC", op_adc, ABSOLUTE_Y_INDEXED, 3, 4},
    [0x61] = { "ADC", op_adc, X_INDEXED_INDIRECT, 2, 6},
    [0x71] = { "ADC", op_adc, INDIRECT_Y_INDEXED, 2, 5},

    // AND (AND Memory with Accumulator)
    [0x29] = { "AND", op_and, IMMEDIATE, 2, 2},
    [0x25] = { "AND", op_and, ZEROPAGE, 2, 3},
    [0x35] = { "AND", op_and, ZEROPAGE_X_INDEXED, 2, 4},
    [0x2D] = { "AND", op_and, ABSOLUTE, 3, 4},
    [0x3D] = { "AND", op_and, ABSOLUTE_X_INDEXED, 3, 4},
    [0x39] = { "AND", op_and, ABSOLUTE_Y_INDEXED, 3, 4},
    [0x21] = { "AND", op_and, X_INDEXED_INDIRECT, 2, 6},
    [0x31] = { "AND", op_and, INDIRECT_Y_INDEXED, 2, 5},

    // ASL Shift Left One Bit (Memory or Accumulator)
    [0x0A] = { "ASL", op_asl, ACCUMULATOR, 1, 2 },
    [0x06] = { "ASL", op_asl, ZEROPAGE, 2, 5 },
    [0x16] = { "ASL", op_asl, ZEROPAGE_X_INDEXED, 2, 6 },
    [0x0E] = { "ASL", op_asl, ABSOLUTE, 3, 6 },
    [0x1E] = { "ASL", op_asl, ABSOLUTE_X_INDEXED, 3, 7 },
    
    //BCC Branch on Carry Clear
    [0x90] = { "BCC", op_bcc, RELATIVE, 2, 2},
    
    //BCS Branch on Carry Set
    [0xB0] = { "BCS", op_bcs, RELATIVE, 2, 2 },

    //BEQ Branch on Result Zero
    [0xF0] = { "BEQ", op_beq, RELATIVE, 2, 2 }, 

    //BIT Test Bits in Memory with Accumulator
    [0x24] = { "BIT", op_bit, ZEROPAGE, 2, 3 },
    [0x2C] = { "BIT", op_bit, ABSOLUTE, 3, 4 },

    //BMI Branch on Result Minus
    [0x30] = { "BMI", op_bmi, RELATIVE, 2, 2 },
  
    //BNE Branch on Result Not Zero
    [0xD0] = { "BNE", op_bne, RELATIVE, 2, 2 },

    //BPL Branch on Result Plus
    [0x10] = { "BPL", op_bpl, RELATIVE, 2, 2 }, 

    //BRK Force Break
    [0x00] = { "BRK", op_brk, IMPLIED, 1, 7 },

    //BVC Branch on Overflow Clear
    [0x50] = { "BVC", op_bvc, RELATIVE, 2, 2},

    //BVS Branch on Overflow Set
    [0x70] = { "BVS", op_bvs, RELATIVE, 2, 2},

    //CLC Clear Carry Flag
    [0x18] = { "CLC", op_clc, IMPLIED, 1, 2},

    //CLD Clear Decimal Mode
    [0xD8] = { "CLD", op_cld, IMPLIED, 1, 2},

    //CLI Clear Interrupt Disable Bit
    [0x58] = { "CLI", op_cli, IMPLIED, 1, 2},

    //CLV Clear Overflow Flag
    [0xB8] = { "CLV", op_clv, IMPLIED, 1, 2},

    //CMP Compare Memory with Accumulator
    [0xC9] = { "CMP", op_cmp, IMMEDIATE, 2, 2},
    [0xC5] = { "CMP", op_cmp, ZEROPAGE, 2, 3},
    [0xD5] = { "CMP", op_cmp, ZEROPAGE_X_INDEXED, 2, 4},
    [0xCD] = { "CMP", op_cmp, ABSOLUTE, 3, 4},
    [0xDD] = { "CMP", op_cmp, ABSOLUTE_X_INDEXED, 3, 4},
    [0xD9] = { "CMP", op_cmp, ABSOLUTE_Y_INDEXED, 3, 4},
    [0xC1] = { "CMP", op_cmp, X_INDEXED_INDIRECT, 2, 6},
    [0xD1] = { "CMP", op_cmp, INDIRECT_Y_INDEXED, 2, 5},

    //CPX Compare Memory and Index X
    [0xE0] = { "CPX", op_cpx, IMMEDIATE, 2, 2},
    [0xE4] = { "CPX", op_cpx, ZEROPAGE,  2, 3 },
    [0xEC] = { "CPX", op_cpx, ABSOLUTE,  3, 4 },

    //CPY Compare Memory and Index Y
    [0xC0] = { "CPY", op_cpy, IMMEDIATE, 2, 2},
    [0xC4] = { "CPY", op_cpy, ZEROPAGE,  2, 3 },
    [0xCC] = { "CPY", op_cpy, ABSOLUTE,  3, 4 },

    //DEC Decrement Memory by One
    [0xC6] = { "DEC", op_dec, ZEROPAGE, 2, 5},
    [0xD6] = { "DEC", op_dec, ZEROPAGE_X_INDEXED, 2, 6},
    [0xCE] = { "DEC", op_dec, ABSOLUTE, 3, 6},
    [0xDE] = { "DEC", op_dec, ABSOLUTE_X_INDEXED, 3, 7},

    //DEX Decrement Index X by One
    [0xCA] = { "DEX", op_dex, IMPLIED, 1, 2},

    //DEY Decrement Index Y by One
    [0x88] = { "DEY", op_dey, IMPLIED, 1, 2},

    //EOR Exclusive-OR Memory with Accumulator
    [0x49] = { "EOR", op_eor, IMMEDIATE, 2, 2},
    [0x45] = { "EOR", op_eor, ZEROPAGE, 2, 3},
    [0x55] = { "EOR", op_eor, ZEROPAGE_X_INDEXED, 2, 4},
    [0x4D] = { "EOR", op_eor, ABSOLUTE, 3, 4},
    [0x5D] = { "EOR", op_eor, ABSOLUTE_X_INDEXED, 3, 4},
    [0x59] = { "EOR", op_eor, ABSOLUTE_Y_INDEXED, 3, 4},
    [0x41] = { "EOR", op_eor, X_INDEXED_INDIRECT, 2, 6},
    [0x51] = { "EOR", op_eor, INDIRECT_Y_INDEXED, 2, 5},

    //INC Increment Memory by One
    [0xE6] = { "INC", op_inc, ZEROPAGE, 2, 5},
    [0xF6] = { "INC", op_inc, ZEROPAGE_X_INDEXED, 2, 6},
    [0xEE] = { "INC", op_inc, ABSOLUTE, 3, 6},
    [0xFE] = { "INC", op_inc, ABSOLUTE_X_INDEXED, 3, 7},

    //INX Increment Index X by One
    [0xE8] = { "INX", op_inx, IMPLIED, 1, 2},

    //INY Increment Index Y by One
    [0xC8] = { "INY", op_iny, IMPLIED, 1, 2},

    //JMP Jump to New Location
    [0x4C] = { "JMP", op_jmp, ABSOLUTE, 3, 3},
    [0x6C] = { "JMP", op_jmp, INDIRECT, 3, 5},

    //JSR Jump to New Location Saving Return Address
    [0x20] = { "JSR", op_jsr, ABSOLUTE, 3, 6},

    //LDA Load Accumulator with Memory
    [0xA9] = { "LDA", op_lda, IMMEDIATE, 2, 2},
    [0xA5] = { "LDA", op_lda, ZEROPAGE, 2, 3 },
    [0xB5] = { "LDA", op_lda, ZEROPAGE_X_INDEXED, 2, 4 },
    [0xAD] = { "LDA", op_lda, ABSOLUTE, 3, 4 },
    [0xBD] = { "LDA", op_lda, ABSOLUTE_X_INDEXED, 3, 4 },
    [0xB9] = { "LDA", op_lda, ABSOLUTE_Y_INDEXED, 3, 4 },
    [0xA1] = { "LDA", op_lda, X_INDEXED_INDIRECT, 2, 6 },
    [0xB1] = { "LDA", op_lda, INDIRECT_Y_INDEXED, 2, 5 },

    //LDX Load Index X with Memory
    [0xA2] = { "LDX", op_ldx, IMMEDIATE, 2, 2},
    [0xA6] = { "LDX", op_ldx, ZEROPAGE, 2, 3 },
    [0xB6] = { "LDX", op_ldx, ZEROPAGE_Y_INDEXED, 2, 4 },
    [0xAE] = { "LDX", op_ldx, ABSOLUTE, 3, 4 },
    [0xBE] = { "LDX", op_ldx, ABSOLUTE_Y_INDEXED, 3, 4 },

    //LDY Load Index X with Memory
    [0xA0] = { "LDY", op_ldy, IMMEDIATE, 2, 2 },
    [0xA4] = { "LDY", op_ldy, ZEROPAGE, 2, 3 },
    [0xB4] = { "LDY", op_ldy, ZEROPAGE_X_INDEXED, 2, 4 },
    [0xAC] = { "LDY", op_ldy, ABSOLUTE, 3, 4 },
    [0xBC] = { "LDY", op_ldy, ABSOLUTE_X_INDEXED, 3, 4 },

    //LSR Shift One Bit Right (Memory or Accumulator)
    [0x4A] = { "LSR", op_lsr, ACCUMULATOR, 1, 2 },
    [0x46] = { "LSR", op_lsr, ZEROPAGE, 2, 5 },
    [0x56] = { "LSR", op_lsr, ZEROPAGE_X_INDEXED, 2, 6 },
    [0x4E] = { "LSR", op_lsr, ABSOLUTE, 3, 6 },
    [0x5E] = { "LSR", op_lsr, ABSOLUTE_X_INDEXED, 3, 7 },

    //NOP No Operation
    [0xEA] = { "NOP", op_nop, IMPLIED, 1, 2 },

    //ORA OR Memory with Accumulator
    [0x09] = { "ORA", op_ora, IMMEDIATE, 2, 2 },
    [0x05] = { "ORA", op_ora, ZEROPAGE, 2, 3 },
    [0x15] = { "ORA", op_ora, ZEROPAGE_X_INDEXED, 2, 4 },
    [0x0D] = { "ORA", op_ora, ABSOLUTE, 3, 4 },
    [0x1D] = { "ORA", op_ora, ABSOLUTE_X_INDEXED, 3, 4 },
    [0x19] = { "ORA", op_ora, ABSOLUTE_Y_INDEXED, 3, 4 },
    [0x01] = { "ORA", op_ora, X_INDEXED_INDIRECT, 2, 6 },
    [0x11] = { "ORA", op_ora, INDIRECT_Y_INDEXED, 2, 5 },

    //PHA Push Accumulator on Stack 
    [0x48] = { "PHA", op_pha, IMPLIED, 1, 3 },

    //PHP Push Processor Status on Stack
    [0x08] = { "PHP", op_php, IMPLIED, 1, 3 },

    //PLA Pull Accumulator from Stack
    [0x68] = { "PLA", op_pla, IMPLIED, 1, 4 },

    //PLP Pull Processor Status from Stack
    [0x28] = { "PLP", op_plp, IMPLIED, 1, 4 },

    //ROL Rotate One Bit Left (Memory or Accumulator)
    [0x2A] = { "ROL", op_rol, ACCUMULATOR, 1, 2 },
    [0x26] = { "ROL", op_rol, ZEROPAGE, 2, 5 },
    [0x36] = { "ROL", op_rol, ZEROPAGE_X_INDEXED, 2, 6 },
    [0x2E] = { "ROL", op_rol, ABSOLUTE, 3, 6 },
    [0x3E] = { "ROL", op_rol, ABSOLUTE_X_INDEXED, 3, 7 },

    //ROR Rotate One Bit Right (Memory or Accumulator)
    [0x6A] = { "ROR", op_ror, ACCUMULATOR, 1, 2 },
    [0x66] = { "ROR", op_ror, ZEROPAGE, 2, 5 },
    [0x76] = { "ROR", op_ror, ZEROPAGE_X_INDEXED, 2, 6 },
    [0x6E] = { "ROR", op_ror, ABSOLUTE, 3, 6 },
    [0x7E] = { "ROR", op_ror, ABSOLUTE_X_INDEXED, 3, 7 },

    //RTI Return from Interrupt
    [0x40] = { "RTI", op_rti, IMPLIED, 1, 6 },
    
    //RTS Return from Subroutine
    [0x60] = { "RTS", op_rts, IMPLIED, 1, 6 },

    //SBC Subtract Memory from Accumulator with Borrow
    [0xE9] = { "SBC", op_sbc, IMMEDIATE, 2, 2 },
    [0xE5] = { "SBC", op_sbc, ZEROPAGE, 2, 3 },
    [0xF5] = { "SBC", op_sbc, ZEROPAGE_X_INDEXED, 2, 4 },
    [0xED] = { "SBC", op_sbc, ABSOLUTE, 3, 4 },
    [0xFD] = { "SBC", op_sbc, ABSOLUTE_X_INDEXED, 3, 4 },
    [0xF9] = { "SBC", op_sbc, ABSOLUTE_Y_INDEXED, 3, 4 },
    [0xE1] = { "SBC", op_sbc, X_INDEXED_INDIRECT, 2, 6 },
    [0xF1] = { "SBC", op_sbc, INDIRECT_Y_INDEXED, 2, 5 },

    //SEC Set Carry Flag
    [0x38] = { "SEC", op_sec, IMPLIED, 1, 2 },

    //SED Set Decimal Flag
    [0xF8] = { "SED", op_sed, IMPLIED, 1, 2 },

    //SEI Set Interrupt Flag
    [0x78] = { "SEI", op_sei, IMPLIED, 1, 2 },

    //STA Store Accumulator in Memory
    [0x85] = { "STA", op_sta, ZEROPAGE, 2, 3},
    [0x95] = { "STA", op_sta, ZEROPAGE_X_INDEXED, 2, 4},
    [0x8D] = { "STA", op_sta, ABSOLUTE, 3, 4},
    [0x9D] = { "STA", op_sta, ABSOLUTE_X_INDEXED, 3, 5},
    [0x99] = { "STA", op_sta, ABSOLUTE_Y_INDEXED, 3, 5},
    [0x81] = { "STA", op_sta, X_INDEXED_INDIRECT, 2, 6},
    [0x91] = { "STA", op_sta, INDIRECT_Y_INDEXED, 2, 6},

    //STX Store Index X in Memory
    [0x86] = { "STX", op_stx, ZEROPAGE, 2, 3},
    [0x96] = { "STX", op_stx, ZEROPAGE_Y_INDEXED, 2, 4},
    [0x8E] = { "STX", op_stx, ABSOLUTE, 3, 4},

    //STY Store Index Y in Memory
    [0x84] = { "STY", op_sty, ZEROPAGE, 2, 3},
    [0x94] = { "STY", op_sty, ZEROPAGE_X_INDEXED, 2, 4},
    [0x8C] = { "STY", op_sty, ABSOLUTE, 3, 4},

    //TAX Transfer Accumulator to Index X
    [0xAA] = { "TAX", op_tax, IMPLIED, 1, 2 },

    //TAY Transfer Accumulator to Index Y
    [0xA8] = { "TAY", op_tay, IMPLIED, 1, 2 },

    //TSX Transfer Stack Pointer to Index X
    [0xBA] = { "TSX", op_tsx, IMPLIED, 1, 2 },

    //TXA Transfer Index X to Accumulator
    [0x8A] = { "TXA", op_txa, IMPLIED, 1, 2 },

    //TXS Transfer Index X to Stack Register
    [0x9A] = { "TXS", op_txs, IMPLIED, 1, 2 },

    //TYA Transfer Index Y to Accumulator
    [0x98] = { "TYA", op_tya, IMPLIED, 1, 2 },

};

static const InstructionInfo not_implemented = {
    "???",
    op_not_implemented,
    IMPLIED,
    1
};

const InstructionInfo *cpu_decode(const uint8_t opcode) {
    if (instruction_table[opcode].handler == NULL) {
        fprintf(stderr, "Unimplemented opcode: %02X\n", opcode);
        return &not_implemented;
    }

    return &instruction_table[opcode];
}

void op_not_implemented(CPU *cpu, Operand *operand) {
    (void)cpu;
    (void)operand;
}

// ADC implementations

void op_adc(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t carry = cpu_get_flag(cpu, C);
    uint8_t accumulator_old = cpu->A;
    uint16_t result_bin = accumulator_old + value + carry;
    uint8_t finalResult_bin = (uint8_t)result_bin;
    cpu_set_flag(cpu, C, result_bin > 0xFF);
    cpu->A = finalResult_bin;
    // Set flags
    cpu_set_flag(cpu, Z, finalResult_bin == 0);
    cpu_set_flag(cpu, N, (finalResult_bin & 0x80) != 0);
    if ((value & 0x80) == (accumulator_old & 0x80))
    {
        cpu_set_flag(cpu, V, (value & 0x80) != (finalResult_bin & 0x80));
    }
    else
    {
        cpu_set_flag(cpu, V, false);
    }
}

void op_and(CPU *cpu, Operand *operand){
    cpu->A &= operand_read(cpu, operand);

    // Set flags
    cpu_set_flag(cpu, Z, cpu->A == 0);
    cpu_set_flag(cpu, N, (cpu->A & 0x80) != 0);
}

void op_asl(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    cpu_set_flag(cpu, C, (value & 0x80) != 0);
    value <<= 1;
    operand_write(cpu, operand, value);
    cpu_set_flag(cpu, N, (value & 0x80) != 0);
    cpu_set_flag(cpu, Z, value == 0);
}

void op_bcc(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, C) == false) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_bcs(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, C) == true) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_beq(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, Z) == true){
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_bit(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    cpu_set_flag(cpu, N, (0x80 & value) != 0);
    cpu_set_flag(cpu, V, (0x40 & value) != 0);
    cpu_set_flag(cpu, Z, (value & cpu->A) == 0);
}

void op_bmi(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, N) == true) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_bne(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, Z) == false) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_bpl(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, N) == false) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_bvc(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, V) == false) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_bvs(CPU *cpu, Operand *operand){
    if (cpu_get_flag(cpu, V) == true) 
    {
        cpu->PC += (int8_t)operand_read(cpu, operand);
    }
}

void op_clc(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, C, false);
}

void op_cld(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, D, false);
}

void op_cli(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, I, false);
}

void op_clv(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, V, false);
}

void op_cmp(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = cpu->A - value;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, C, cpu->A >= value); //Borrow one if M > A
    cpu_set_flag(cpu, Z, (result == 0));
}

void op_cpx(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = cpu->X - value;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, C, cpu->X >= value); //Borrow one if M > X
    cpu_set_flag(cpu, Z, (result == 0));
}

void op_cpy(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = cpu->Y - value;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, C, cpu->Y >= value); //Borrow one if M > Y
    cpu_set_flag(cpu, Z, (result == 0));
}

void op_dec(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = value - 1;
    operand_write(cpu, operand, result);
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_dex(CPU *cpu, Operand *operand){
    uint8_t value = cpu->X;
    uint8_t result = value - 1;
    cpu->X = result;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_dey(CPU *cpu, Operand *operand){
    uint8_t value = cpu->Y;
    uint8_t result = value - 1;
    cpu->Y = result;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_eor(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = cpu->A ^ value;
    cpu->A = result;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_inc(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = value + 1;
    operand_write(cpu, operand, result);
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_inx(CPU *cpu, Operand *operand){
    uint8_t value = cpu->X;
    uint8_t result = value + 1;
    cpu->X = result;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}
void op_iny(CPU *cpu, Operand *operand){
    uint8_t value = cpu->Y;
    uint8_t result = value + 1;
    cpu->Y = result;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_jmp(CPU *cpu, Operand *operand){
    cpu->PC = operand->loc;
}

void op_jsr(CPU *cpu, Operand *operand){
    uint16_t ret = cpu->PC - 1;
    uint8_t byte_HH = 0xFF & ((ret) >> 8);
    uint8_t byte_LL = 0xFF & (ret);
    cpu_push_byte(cpu, byte_HH);
    cpu_push_byte(cpu, byte_LL);
    cpu->PC = operand->loc;
}

void op_lda(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    cpu->A = value;
    cpu_set_flag(cpu, N, (0x80 & value) != 0);
    cpu_set_flag(cpu, Z, value == 0);
}

void op_ldx(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    cpu->X = value;
    cpu_set_flag(cpu, N, (0x80 & value) != 0);
    cpu_set_flag(cpu, Z, value == 0);
}

void op_ldy(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    cpu->Y = value;
    cpu_set_flag(cpu, N, (0x80 & value) != 0);
    cpu_set_flag(cpu, Z, value == 0);
}

void op_lsr(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = value >> 1;
    cpu_set_flag(cpu, C, 0x01 & value);
    operand_write(cpu, operand, result);
    cpu_set_flag(cpu, N, 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_ora(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t result = value | cpu-> A;
    cpu->A = result;
    cpu_set_flag(cpu, N, (0x80 & result) != 0);
    cpu_set_flag(cpu, Z, result == 0);
}

void op_pha(CPU *cpu, Operand *operand){
    cpu_push_byte(cpu, cpu->A);
}

void op_php(CPU *cpu, Operand *operand){
    uint8_t status = (cpu->P) | (B | UNUSED);
    cpu_push_byte(cpu, status);
}

void op_pla(CPU *cpu, Operand *operand){
    cpu->A = cpu_pull_byte(cpu);
    cpu_set_flag(cpu, N, (0x80 & cpu->A) != 0);
    cpu_set_flag(cpu, Z, cpu->A == 0);
}

void op_plp(CPU *cpu, Operand *operand){
    uint8_t status = cpu_pull_byte(cpu);
    status &= ~B;        // break siempre a 0
    status |= UNUSED;    // unused siempre a 1
    cpu->P = status;
    cpu->P = status;
}

void op_rol(CPU *cpu, Operand *operand){
    uint8_t val = operand_read(cpu, operand);
    uint8_t newVal = (val << 1) | cpu_get_flag(cpu, C);
    cpu_set_flag(cpu, C, 0x80 & val);
    operand_write(cpu, operand, newVal);
    cpu_set_flag(cpu, N, (0x80 & newVal) != 0);
    cpu_set_flag(cpu, Z, newVal == 0);
}

void op_ror(CPU *cpu, Operand *operand){
    uint8_t val = operand_read(cpu, operand);
    uint8_t newVal = (val >> 1) | (cpu_get_flag(cpu, C) << 7);
    cpu_set_flag(cpu, C, 0x01 & val);
    operand_write(cpu, operand, newVal);
    cpu_set_flag(cpu, N, (0x80 & newVal) != 0);
    cpu_set_flag(cpu, Z, newVal == 0);
}

void op_rti(CPU *cpu, Operand *operand){
    uint8_t status = cpu_pull_byte(cpu);
    status &= ~B;        // break siempre a 0
    status |= UNUSED;    // unused siempre a 1
    cpu->P = status;
    uint8_t pc_LL = cpu_pull_byte(cpu);
    uint8_t pc_HH = cpu_pull_byte(cpu);
    cpu->PC = (uint16_t)(pc_HH << 8) | pc_LL;
}

void op_rts(CPU *cpu, Operand *operand){
    uint8_t pc_LL = cpu_pull_byte(cpu);
    uint8_t pc_HH = cpu_pull_byte(cpu);
    cpu->PC = (uint16_t)(pc_HH << 8) | pc_LL;
    cpu->PC += 1;
}

void op_sbc(CPU *cpu, Operand *operand){
    uint8_t value = operand_read(cpu, operand);
    uint8_t carry = cpu_get_flag(cpu, C);
    uint8_t accumulator_old = cpu->A;
    uint16_t result_bin = (uint8_t)~value + accumulator_old + carry;

    uint8_t finalResult_bin = (uint8_t)result_bin;
    cpu->A = finalResult_bin;

    // Set flags
    cpu_set_flag(cpu, Z, finalResult_bin == 0);
    cpu_set_flag(cpu, N, (finalResult_bin & 0x80) != 0);
    cpu_set_flag(cpu, C, result_bin > 0xFF);
    if ((~value & 0x80) == (accumulator_old & 0x80))
    {
        cpu_set_flag(cpu, V, (~value & 0x80) != (finalResult_bin & 0x80));
    }
    else
    {
        cpu_set_flag(cpu, V, false);
    }
}

void op_sec(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, C, true);
}

void op_sed(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, D, true);
}

void op_sei(CPU *cpu, Operand *operand){
    cpu_set_flag(cpu, I, true);
}

void op_sta(CPU *cpu, Operand *operand){
    operand_write(cpu, operand, cpu->A);
}

void op_stx(CPU *cpu, Operand *operand){
    operand_write(cpu, operand, cpu->X);
}

void op_sty(CPU *cpu, Operand *operand){
    operand_write(cpu, operand, cpu->Y);
}


void op_tax(CPU *cpu, Operand *operand){
    cpu->X = cpu->A;
    cpu_set_flag(cpu, Z, cpu->A == 0);
    cpu_set_flag(cpu, N, (cpu->A & 0x80) != 0);
}

void op_tay(CPU *cpu, Operand *operand){
    cpu->Y = cpu->A;
    cpu_set_flag(cpu, Z, cpu->A == 0);
    cpu_set_flag(cpu, N, (cpu->A & 0x80) != 0);
}

void op_tsx(CPU *cpu, Operand *operand){
    cpu->X = cpu->S;
    cpu_set_flag(cpu, Z, cpu->S == 0);
    cpu_set_flag(cpu, N, (cpu->S & 0x80) != 0);
}

void op_txa(CPU *cpu, Operand *operand){
    cpu->A = cpu->X;
    cpu_set_flag(cpu, Z, cpu->X == 0);
    cpu_set_flag(cpu, N, (cpu->X & 0x80) != 0);
}

void op_txs(CPU *cpu, Operand *operand){
    cpu->S = cpu->X;
}

void op_tya(CPU *cpu, Operand *operand){
    cpu->A = cpu->Y;
    cpu_set_flag(cpu, Z, cpu->Y == 0);
    cpu_set_flag(cpu, N, (cpu->Y & 0x80) != 0);
}

void op_nop(CPU *cpu, Operand *operand){
    
}

void op_brk(CPU *cpu, Operand *operand){
    //Save PC to recover
    cpu->PC+=1;
    cpu_interrupt(cpu, VECTOR_IRQ, true);
}
