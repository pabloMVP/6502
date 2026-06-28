#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "bus.h"


int main(int argc, char *argv[])
{
    
    // Initialize bus and cpu
    Bus bus = {0};
    CPU cpu = {
        .bus = &bus,
    };
    
    FILE *file = fopen(argv[1], "rb");  

    if (file == NULL) {
        printf("Error reading the binary!\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);           //Move to end of file
    size_t binarySize = ftell(file);    //Bin size
    rewind(file);           //Back to the beginning
    
    uint8_t data[binarySize];
    fread(data, sizeof(uint8_t), binarySize, file);
    //Load to the bus
    bus_load(cpu.bus, 0, data, binarySize);
    
    //Prepare CPU to run
    cpu_reset(&cpu);
    cpu.PC = 0x0400;

    uint16_t SUCCESS = 0x3469;

    size_t iters = 0;
    for (;;) {
        uint16_t pc_old = cpu.PC;
        cpu_step(&cpu);
        if (cpu.PC == pc_old){
            if (cpu.PC == SUCCESS){
               printf("TEST PASSED!!\n");
               return 0;
            }
            else {
                printf("Test fell at a trap in %04x\n", cpu.PC);  // trap detected
                return 1;
            }
        }
        if (iters > 100000000){
            printf("Max iters!");
            return 1;
        }
        iters++;
    }
    return 0;

}
