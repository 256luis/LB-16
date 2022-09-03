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

void print_bytes(RawInstruction* arr, size_t size)
{
    for (int i = 0; i < size; i++)
    {        
        Word op1 = { .u = arr[i].operand_1 };
        Word op2 = { .u = arr[i].operand_2 };
    
        printf("%02x %02x %02x %02x %02x ",
               arr[i].opcode,
               op1.bytes[0].u,
               op1.bytes[1].u,
               op2.bytes[0].u,
               op2.bytes[1].u
        );
    }
    putchar('\n');
}

int main(int argc, char** argv)
{
    RawInstruction prog[] = {
        {MOVL, REG_AH, 0x5e},
        {MOVL, REG_BL, 0x4f},
        {MOVL, REG_BH, 0xa3},
        {MOVL, REG_CX, 0x49eb},
        {HLT, 0, 0}
    };
    print_bytes(prog, 5);
    INSIST(argc > 1, "no file specified\n");

    // sdl stuff
    
    CPU* cpu = cpu_init();
    Instruction* program = load_program(argv[1]);

    while (!cpu->should_halt)
    {
        execute_instruction(cpu, program[cpu->reg_ip]);     
    }
    
    cpu_dump(cpu);
    
    // cleanup
    free(cpu);
    free(program);
    return 1;
}
