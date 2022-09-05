#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vm.h"
#include "helper.h"

// temporary until we have an assembler
typedef struct RawInstruction
{
    uint8_t opcode;
    uint16_t operand_1;
    uint16_t operand_2;
} RawInstruction;

// this is also temporary
void write_bytes(RawInstruction* arr, size_t size)
{
    FILE* file = fopen("sample.bin", "wb");

    // write magic number
    uint8_t magic_number[] = {0xd0, 0xd0, 0xfa, 0xce, 0x16};
    for (int i = 0; i < MAGIC_NUMBER_LENGTH; i++)
    {
        fputc(magic_number[i], file);
    }
    
    for (size_t i = 0; i < size; i++)
    {        
        Word op1 = { .u = arr[i].operand_1 };
        Word op2 = { .u = arr[i].operand_2 };

        fprintf(file, "%c%c%c%c%c",
               arr[i].opcode,
               op1.bytes[0].u,
               op1.bytes[1].u,
               op2.bytes[0].u,
               op2.bytes[1].u
        );
    }
    fclose(file);
}

int main(int argc, char** argv)
{
    RawInstruction prog[] = {        
        {OUTL, 'H', 0},
        {OUTL, 'e', 0},
        {OUTL, 'l', 0},
        {OUTL, 'l', 0},
        {OUTL, 'o', 0},
        {OUTL, ',', 0},
        {OUTL, ' ', 0},
        {OUTL, 'W', 0},
        {OUTL, 'o', 0},
        {OUTL, 'r', 0},
        {OUTL, 'l', 0},
        {OUTL, 'd', 0},
        {OUTL, '!', 0},
        {OUTL, '\n', 0},
        {HLT, 0, 0}
    };
    write_bytes(prog, sizeof(prog) / sizeof(RawInstruction));
    INSIST(argc > 1, "no file specified\n");

    // sdl stuff
    
    CPU* cpu = cpu_init();
    Instruction* program = load_program(argv[1]);

    while (!cpu->should_halt)
    {
        execute_instruction(cpu, program[cpu->reg_ip]);     
    }
    
    // cpu_dump(cpu);
    
    // cleanup
    free(cpu);
    free(program);
    return 1;
}
