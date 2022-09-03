#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "general.h"
#include "instruction.h"

#define MEMORY_STACK_SIZE 32 // arbitrary
#define CALL_STACK_SIZE 32 // arbitrary

typedef enum Register
{
    REG_A, REG_B, REG_C, REG_D,    
    REG_COUNT
} Register;

typedef struct CPU
{
    Word registers[REG_COUNT];
    uint16_t reg_ip;
    uint16_t reg_sp;
    Word reg_r;
    
    Word memory_stack[MEMORY_STACK_SIZE];
    uint16_t call_stack[CALL_STACK_SIZE];
    bool should_halt;
} CPU;

CPU* cpu_init();
void cpu_dump(CPU* cpu);
void execute_instruction(CPU* cpu, Instruction inst);

#endif
