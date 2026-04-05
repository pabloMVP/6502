#include "cpu.h"

#define STUB_HANDLER(name) \
    void name(CPU *cpu) { \
        op_not_implemented(cpu); \
    }

STUB_HANDLER(op_and_immediate)
STUB_HANDLER(op_and_zeropage)
STUB_HANDLER(op_and_zeropage_x)
STUB_HANDLER(op_and_absolute)
STUB_HANDLER(op_and_absolute_X)
STUB_HANDLER(op_and_absolute_Y)
STUB_HANDLER(op_and_indirect_X)
STUB_HANDLER(op_and_indirect_Y)

STUB_HANDLER(op_asl_accumulator)
STUB_HANDLER(op_asl_zeropage)
STUB_HANDLER(op_asl_zeropage_x)
STUB_HANDLER(op_asl_absolute)
STUB_HANDLER(op_asl_absolute_X)

STUB_HANDLER(op_bcc_relative)
STUB_HANDLER(op_bcs_relative)
STUB_HANDLER(op_beq_relative)
STUB_HANDLER(op_bit_zeropage)
STUB_HANDLER(op_bit_absolute)
STUB_HANDLER(op_bmi_relative)
STUB_HANDLER(op_bne_relative)
STUB_HANDLER(op_brk_implied)
STUB_HANDLER(op_bvc_relative)
STUB_HANDLER(op_bpl_relative)

