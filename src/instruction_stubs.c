#include "cpu.h"

#define STUB_HANDLER(name) \
    void name(CPU *cpu) { \
        op_not_implemented(cpu) ; \
    }

STUB_HANDLER(op_and_immediate)


