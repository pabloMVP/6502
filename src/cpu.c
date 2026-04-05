#include "cpu.h"
#include <stddef.h>

void cpu_reset(CPU *cpu){
    cpu->PC = (uint16_t)bus_read(cpu->bus, 0xFFFC) | ((uint16_t)bus_read(cpu->bus, 0xFFFD) << 8);
    cpu->S = 0xFF;
    cpu->A = cpu->X = cpu->Y = cpu->P = 0x00; //we choose zero initialization as our own policy, given there is no defined behaviour for this
}

uint8_t cpu_fetch(CPU *cpu){
    uint8_t opcode = bus_read(cpu->bus, cpu->PC);
    cpu->PC+=1;
    return opcode;
}

const InstructionInfo* cpu_decode(CPU *cpu){
    uint8_t opcode = cpu_fetch(cpu);
    const InstructionInfo *instruction = cpu_decode(opcode);
    return instruction;
}

void cpu_execute(CPU *cpu, ADDRESS_MODE address_mode, InstructionHandler* handler){
    switch(address_mode)
    {
        case ACCUMULATOR:
            (*handler)(cpu);
            break;
        case ABSOLUTE:
            uint8_t address_LL = cpu_fetch(cpu);
            uint8_t address_HH = cpu_fetch(cpu);
            uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
            uint8_t operand = bus_read(cpu->bus, address);
            (*handler)(cpu, operand);
            break;
    }
}

void cpu_set_flag(CPU *cpu, STATUS_FLAGS flag, bool set){
    if (set)
    {
        cpu->P |= flag;
    }
    else 
    {
        cpu->P &= ~flag;
    }
}

bool cpu_get_flag(const CPU *cpu, STATUS_FLAGS flag){
    return cpu->P & flag;
}

