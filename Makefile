CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iincludes

SRC = src/bus.c src/cpu.c src/instruction_table.c src/instruction_stubs.c
TEST_SRC = tests/test_main.c tests/test_adc.c

TEST_BIN = bin/tests

.PHONY: test clean

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(SRC) $(TEST_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $(TEST_BIN)

clean:
	rm -rf bin
