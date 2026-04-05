#include "cpu.h"

void test_op_adc_immediate_basic(void);
void test_op_adc_immediate_carry_zero(void);
void test_op_adc_immediate_overflow_signed(void);

int main(void) {
    test_op_adc_immediate_basic();
    test_op_adc_immediate_carry_zero();
    test_op_adc_immediate_overflow_signed();
    return 0;
}