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
            (*handler)(cpu, &(cpu->A));
            break;
        case IMMEDIATE:
            uint8_t operand = cpu_fetch(cpu);
            (*handler)(cpu, &operand);
            break;
        case ZEROPAGE:
            uint8_t address = cpu_fetch(cpu);
            uint8_t operand = bus_read(cpu->bus, address);
            (*handler)(cpu, &operand);
            break;
        case ZEROPAGE_X_INDEXED:
            uint8_t address = cpu_fetch(cpu);
            address += cpu->X;
            uint8_t operand = bus_read(cpu->bus, address);
            (*handler)(cpu, &operand);
            break;
        case ABSOLUTE:
            uint8_t address_LL = cpu_fetch(cpu);
            uint8_t address_HH = cpu_fetch(cpu);
            uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
            uint8_t operand = bus_read(cpu->bus, address);
            (*handler)(cpu, &operand);
            break;
        case ABSOLUTE_X_INDEXED:
            uint8_t address_LL = cpu_fetch(cpu);
            uint8_t address_HH = cpu_fetch(cpu);
            uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
            uint8_t operand = bus_read(cpu->bus, address + cpu->X);
            (*handler)(cpu, &operand);
            break;
        case ABSOLUTE_Y_INDEXED:
            uint8_t address_LL = cpu_fetch(cpu);
            uint8_t address_HH = cpu_fetch(cpu);
            uint16_t address = (uint16_t)(address_HH << 8) | address_LL; 
            uint8_t operand = bus_read(cpu->bus, address + cpu->Y);
            (*handler)(cpu, &operand);
            break;
        case X_INDEXED_INDIRECT:
            uint8_t indirect_address_LL = cpu_fetch(cpu);
            uint8_t final_address_LL = bus_read(cpu->bus, (indirect_address_LL + cpu->X) & 0xFF); 
            uint8_t final_address_HH = bus_read(cpu->bus, (indirect_address_LL + cpu->X + 1) & 0xFF); 
            uint8_t operand = bus_read(cpu->bus, (uint16_t)(final_address_HH << 8) | final_address_LL);
            (*handler)(cpu, &operand);
            break;
        case INDIRECT_Y_INDEXED:
            uint8_t indirect_address_LL = cpu_fetch(cpu);
            uint8_t final_address_LL = bus_read(cpu->bus, indirect_address_LL & 0xFF); 
            uint8_t final_address_HH = bus_read(cpu->bus, (indirect_address_LL + 1) & 0xFF); 
            uint8_t operand = bus_read(cpu->bus, ((uint16_t)(final_address_HH << 8) | final_address_LL) + cpu->Y);
            (*handler)(cpu, &operand);
            break;
        case RELATIVE:
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

