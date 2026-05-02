#include <assert.h>
#include "cpu.h"


void test_op_adc_immediate_basic(void){
    Bus bus = {0};
    CPU cpu = {0};
    cpu.bus = &bus;

    cpu.PC = 0x8000;
    cpu.A = 0x10;
    cpu.P = 0x00;
    bus_write(cpu.bus, 0x8000, 0x05);
    cpu_execute(&cpu, IMMEDIATE, op_adc);
    assert(cpu.A == 0x15);
    assert(cpu.PC == 0x8001);
    assert(cpu_get_flag(&cpu, C) == false);
    assert(cpu_get_flag(&cpu, Z) == false);

}

void test_op_adc_immediate_carry_zero(void){
    Bus bus = {0};
    CPU cpu = {0};
    cpu.bus = &bus;

    cpu.PC = 0x8000;
    cpu.A = 0xFF;
    cpu.P = 0x00;
    bus_write(cpu.bus, 0x8000, 0x01);
    cpu_execute(&cpu, IMMEDIATE, op_adc);
    assert(cpu.A == 0x00);
    assert(cpu.PC == 0x8001);
    assert(cpu_get_flag(&cpu, C) == true);
    assert(cpu_get_flag(&cpu, Z) == true);
    assert(cpu_get_flag(&cpu, N) == false);
    assert(cpu_get_flag(&cpu, V) == false);
}

void test_op_adc_immediate_overflow_signed(void){
    Bus bus = {0};
    CPU cpu = {0};
    cpu.bus = &bus;

    cpu.PC = 0x8000;
    cpu.A = 0x50;
    cpu.P = 0x00;
    bus_write(cpu.bus, 0x8000, 0x50);
    cpu_execute(&cpu, IMMEDIATE, op_adc);
    assert(cpu.A == 0xA0);
    assert(cpu.PC == 0x8001);
    assert(cpu_get_flag(&cpu, V) == true);
    assert(cpu_get_flag(&cpu, N) == true);
    assert(cpu_get_flag(&cpu, C) == false);
    assert(cpu_get_flag(&cpu, Z) == false);
}
