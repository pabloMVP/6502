# 6502 emulator

A functionally complete emulator of the MOS 6502 CPU, written in C. All 151
official opcodes are implemented across every addressing mode, including BCD
(decimal) mode and the BRK/IRQ/NMI interrupt sequence.

It **passes Klaus Dormann's [6502 functional test](https://github.com/Klaus2m5/6502_65C02_functional_tests)**
in full.

## Design

- **`Bus`** (`bus.c`/`bus.h`) — flat 64 KB address space. `bus_read`/`bus_write`
  for access, `bus_load` to copy a block of bytes into memory.
- **`CPU`** (`cpu.c`/`cpu.h`) — registers, flags, fetch/decode/execute and the
  interrupt logic. Addressing modes are resolved in `cpu_execute`, which builds
  an `Operand` (accumulator or effective address). Handlers read and write the
  operand uniformly through `operand_read`/`operand_write`, so the same handler
  works for accumulator, immediate and memory modes.
- **`instruction_table.c`** — the 256-entry opcode table and every instruction
  handler.

## Build & test

Requires a C11 compiler and `make`.

```sh
make test         # build and run the unit tests
make functional   # build and run Klaus' functional test (prints TEST PASSED!!)
make clean
```

The functional test lives in a git submodule, so clone with:

```sh
git clone --recursive <repo-url>
# or, if already cloned:
git submodule update --init --recursive
```

## Running a binary

```sh
make bin/emu
./bin/emu path/to/program.bin
```

The image is loaded at `0x0000`; the run loop steps until the program counter
gets stuck (a `JMP *` trap) or a step cap is reached.

## Roadmap

- Cycle accuracy (per-instruction cycles, page-cross and branch penalties).
- Refactor the bus into a region-dispatching memory map (RAM mirroring, I/O,
  cartridge via mappers).
- NES target: PPU, APU, controllers and mappers, with decimal mode disabled
  (the 2A03 ignores it) behind a build flag.
