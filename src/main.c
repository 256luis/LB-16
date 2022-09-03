#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vm.h"
#include "helper.h"

int main(int argc, char** argv)
{
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
