#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "bus.h"
#include "cartridge.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("usage: %s <rom.nes> <nestest.log>\n", argv[0]);
        return 1;
    }

    // Initialize bus and cpu
    Bus bus = {0};
    CPU cpu = { .bus = &bus };

    FILE *binary = fopen(argv[1], "rb");
    FILE *testlog = fopen(argv[2], "r");
    if (binary == NULL || testlog == NULL) {
        printf("Error opening the ROM or the log\n");
        return 1;
    }

    fseek(binary, 0, SEEK_END);           // Move to end of file
    size_t binarySize = ftell(binary);    // Binary size
    rewind(binary);                       // Back to the beginning

    uint8_t *data = malloc(binarySize);
    fread(data, 1, binarySize, binary);
    fclose(binary);

    // Load the cartridge
    Cartridge cart = cartridge_loader(data);
    free(data);                 // cartridge_loader already memcpy'd PRG/CHR
    if (cart.prg_rom == NULL) {
        printf("Failed loading the cartridge\n");
        return 1;
    }
    bus.cart = &cart;

    // Prepare CPU to run. nestest's automated mode starts at 0xC000.
    cpu_reset(&cpu);
    cpu.PC = 0xC000;

    size_t line = 0;
    uint8_t a_test, x_test, y_test, p_test;
    uint16_t pc_test;
    char buf[256];

    // The log describes the CPU state *before* each instruction, so we
    // compare against the current state and only then cpu_step().
    for (;;) {
        if (fgets(buf, sizeof(buf), testlog) == NULL) {
            printf("nestest: all %zu lines matched\n", line);
            break;
        }
        char *regs = strstr(buf, "A:");
        if (regs == NULL) break;
        if (sscanf(buf, "%4hx", &pc_test) != 1) break;
        if (sscanf(regs, "A:%hhx X:%hhx Y:%hhx P:%hhx",
                   &a_test, &x_test, &y_test, &p_test) != 4) break;
        line++;

        if (cpu.PC != pc_test || cpu.A != a_test || cpu.X != x_test ||
            cpu.Y != y_test || cpu.P != p_test) {
            printf("Mismatch at log line %zu:\n", line);
            printf("  CPU: PC:%04X A:%02X X:%02X Y:%02X P:%02X\n",
                   cpu.PC, cpu.A, cpu.X, cpu.Y, cpu.P);
            printf("  LOG: PC:%04X A:%02X X:%02X Y:%02X P:%02X\n",
                   pc_test, a_test, x_test, y_test, p_test);
            break;
        }

        cpu_step(&cpu);
    }

    fclose(testlog);
    cartridge_free(bus.cart);
    return 0;
}
