# Next Steps

## Current state

- The project is a MOS 6502 emulator in C.
- `Bus` is modeled as a flat 64 KB memory space.
- `CPU` contains registers `PC`, `S`, `A`, `X`, `Y`, `P` and a pointer to `Bus`.
- Instruction metadata lives in `src/instruction_table.c`.
- There is an initial test scaffold in `tests/`.
- Temporary stubs for unimplemented handlers live in `src/instruction_stubs.c`.

## What is already implemented

- `cpu_reset`
- `cpu_fetch`
- `cpu_get_flag`
- `cpu_set_flag`
- `ADC` common logic
- `ADC` variants:
  - immediate
  - zeropage
  - zeropage,X
  - absolute
  - absolute,X
  - absolute,Y
  - (indirect,X)
  - (indirect),Y

## Tests that already exist

- `test_op_adc_immediate_basic`
- `test_op_adc_immediate_carry_zero`
- `test_op_adc_immediate_overflow_signed`

## Important note

- The repo currently contains a `WIP` refactor around `cpu_execute` / `cpu_decode`.
- The current `main` branch includes this in-progress design work on purpose.
- If resuming work, review:
  - `includes/cpu.h`
  - `src/cpu.c`
  - `src/instruction_table.c`

## Design decisions taken so far

- Status flags are stored in `P` as a bitfield.
- Flags are manipulated through `cpu_set_flag` and `cpu_get_flag`.
- Instruction metadata is stored in a lookup table by opcode.
- A temporary “one handler per variant” approach was used first.
- A possible refactor is being explored:
  - decode opcode
  - resolve operand from `ADDRESS_MODE`
  - execute common instruction logic with a normalized operand

## Recommended next step

Decide whether to:

1. keep the current per-variant handlers for now and continue implementing instructions, or
2. finish the `cpu_execute` refactor before adding more instructions.

If choosing option 2, first make `src/cpu.c` compile again consistently with the `InstructionHandler` type.
