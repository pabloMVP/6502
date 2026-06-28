CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iincludes

SRC = src/bus.c src/cpu.c src/instruction_table.c src/instruction_stubs.c
TEST_SRC = tests/test_main.c tests/test_adc.c

TEST_BIN = bin/tests

EMU_BIN = bin/emu
KLAUS_BIN = external/klaus_tests/bin_files/6502_functional_test.bin

.PHONY: test functional clean

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(SRC) $(TEST_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $(TEST_BIN)

functional: $(EMU_BIN)
	./$(EMU_BIN) $(KLAUS_BIN)

$(EMU_BIN): $(SRC) src/main.c
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) src/main.c -o $(EMU_BIN)

clean:
	rm -rf bin
