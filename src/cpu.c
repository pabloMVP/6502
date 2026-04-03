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

void cpu_step(CPU *cpu){
    uint8_t opcode = cpu_fetch(cpu);
    const InstructionInfo *instruction = cpu_decode(opcode);

    if (instruction->handler != NULL) {
        instruction->handler(cpu);
    }
}
