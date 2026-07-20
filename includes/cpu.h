#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>
#include "bus.h"

typedef struct CPU CPU;

typedef enum {
    C = 0x1,
    Z = 0x2,
    I = 0x4,
    D = 0x8,
    B = 0x10,
    UNUSED = 0x20,
    V = 0x40,
    N = 0x80
} STATUS_FLAGS;

typedef enum {
    ACCUMULATOR,
    ABSOLUTE,
    ABSOLUTE_X_INDEXED,
    ABSOLUTE_Y_INDEXED,
    IMMEDIATE,
    IMPLIED,
    INDIRECT,
    X_INDEXED_INDIRECT,
    INDIRECT_Y_INDEXED,
    RELATIVE,
    ZEROPAGE,
    ZEROPAGE_X_INDEXED,
    ZEROPAGE_Y_INDEXED
} ADDRESS_MODE;

typedef enum {
    VECTOR_NMI = 0xFFFA,
    VECTOR_RESET = 0xFFFC,
    VECTOR_IRQ = 0xFFFE // both for IRQ and BRK
} SYSTEM_VECTOR;

typedef struct {
    bool isAccumulator;
    uint16_t loc;
} Operand;

uint8_t operand_read(CPU *cpu, Operand *operand);
void operand_write(CPU *cpu, Operand *operand, uint8_t val);

typedef void (*InstructionHandler)(CPU *cpu, Operand *operand);

typedef struct {
    const char *mnemonic;
    InstructionHandler handler;
    ADDRESS_MODE address_mode;
    uint8_t bytes;
    uint8_t base_cycles;
} InstructionInfo;

struct CPU {
    uint16_t PC; // program counter
    uint8_t S; // stack pointer
    uint8_t A, X, Y; // registers
    uint8_t P; // processor status flags
    bool irq;
    bool nmi;
    Bus* bus;
};

void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);
uint8_t cpu_fetch(CPU *cpu);
const InstructionInfo *cpu_decode(uint8_t opcode);
void cpu_execute(CPU *cpu, ADDRESS_MODE address_mode, InstructionHandler handler);
void cpu_interrupt(CPU *cpu, SYSTEM_VECTOR vector, bool set_break);

void cpu_push_byte(CPU *cpu, uint8_t byte);
uint8_t cpu_pull_byte(CPU *cpu); 

bool cpu_get_flag(const CPU *cpu, STATUS_FLAGS flag);
void cpu_set_flag(CPU *cpu, STATUS_FLAGS flag, bool set);


// ADC (Add Memory to Accumulator with Carry)
void op_adc(CPU *cpu, Operand *operand);
// AND (AND Memory with Accumulator)
void op_and(CPU *cpu, Operand *operand); 
// ASL (Shift Left One Bit (Memory or Accumulator))
void op_asl(CPU *cpu, Operand *operand);
// BCC (Branch on Carry Clear)
void op_bcc(CPU *cpu, Operand *operand);
void op_bcs(CPU *cpu, Operand *operand);
void op_beq(CPU *cpu, Operand *operand);
void op_bit(CPU *cpu, Operand *operand);
void op_bmi(CPU *cpu, Operand *operand);
void op_bne(CPU *cpu, Operand *operand);
void op_bpl(CPU *cpu, Operand *operand);
void op_brk(CPU *cpu, Operand *operand);
void op_bvc(CPU *cpu, Operand *operand);
void op_bvs(CPU *cpu, Operand *operand);
void op_clc(CPU *cpu, Operand *operand);
void op_cld(CPU *cpu, Operand *operand);
void op_cli(CPU *cpu, Operand *operand);
void op_clv(CPU *cpu, Operand *operand);
void op_cmp(CPU *cpu, Operand *operand);
void op_lda(CPU *cpu, Operand *operand);
void op_tax(CPU *cpu, Operand *operand);
void op_cpy(CPU *cpu, Operand *operand);
void op_cpx(CPU *cpu, Operand *operand);
void op_dec(CPU *cpu, Operand *operand);
void op_dex(CPU *cpu, Operand *operand);
void op_dey(CPU *cpu, Operand *operand);
void op_eor(CPU *cpu, Operand *operand);
void op_inc(CPU *cpu, Operand *operand);
void op_inx(CPU *cpu, Operand *operand);
void op_iny(CPU *cpu, Operand *operand);
void op_jmp(CPU *cpu, Operand *operand);
void op_jsr(CPU *cpu, Operand *operand);
void op_lda(CPU *cpu, Operand *operand);
void op_ldx(CPU *cpu, Operand *operand);
void op_ldy(CPU *cpu, Operand *operand);
void op_lsr(CPU *cpu, Operand *operand);
void op_ora(CPU *cpu, Operand *operand);
void op_pha(CPU *cpu, Operand *operand);
void op_php(CPU *cpu, Operand *operand);
void op_pla(CPU *cpu, Operand *operand);
void op_plp(CPU *cpu, Operand *operand);
void op_rol(CPU *cpu, Operand *operand);
void op_ror(CPU *cpu, Operand *operand);
void op_rti(CPU *cpu, Operand *operand);
void op_rts(CPU *cpu, Operand *operand);
void op_sbc(CPU *cpu, Operand *operand);
void op_sec(CPU *cpu, Operand *operand);
void op_sed(CPU *cpu, Operand *operand);
void op_sei(CPU *cpu, Operand *operand);
void op_sta(CPU *cpu, Operand *operand);
void op_stx(CPU *cpu, Operand *operand);
void op_sty(CPU *cpu, Operand *operand);
void op_tax(CPU *cpu, Operand *operand);
void op_tay(CPU *cpu, Operand *operand);
void op_tsx(CPU *cpu, Operand *operand);
void op_txa(CPU *cpu, Operand *operand);
void op_txs(CPU *cpu, Operand *operand);
void op_tya(CPU *cpu, Operand *operand);
void op_nop(CPU *cpu, Operand *operand);
void op_not_implemented(CPU *cpu, Operand *operand);

void op_and_immediate(CPU *cpu, Operand *operand);

#endif // CPU_H
