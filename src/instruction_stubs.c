#include "cpu.h"

#define STUB_HANDLER(name) \
    void name(CPU *cpu, uint8_t *operand) { \
        op_not_implemented(cpu, operand) ; \
    }

STUB_HANDLER(op_and_immediate)


