CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iincludes

SRC = src/bus.c src/cpu.c src/instruction_table.c src/instruction_stubs.c src/cartridge.c src/ppu.c
TEST_SRC = tests/test_main.c tests/test_adc.c

TEST_BIN = bin/tests
EMU_BIN = bin/emu

NESTEST_ROM = tests/roms/nestest.nes
NESTEST_LOG = tests/logs/nestest.log

.PHONY: test nestest clean

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(SRC) $(TEST_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $(TEST_BIN)

nestest: $(EMU_BIN)
	./$(EMU_BIN) $(NESTEST_ROM) $(NESTEST_LOG)

$(EMU_BIN): $(SRC) src/main.c
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) src/main.c -o $(EMU_BIN)

clean:
	rm -rf bin
