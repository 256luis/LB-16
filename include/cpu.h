#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "general.h"
#include "instruction.h"

#define MEMORY_STACK_SIZE 32 // arbitrary
#define CALL_STACK_SIZE 32 // arbitrary
#define REG_COUNT 4

typedef enum Register
{
    REG_AX, REG_AH, REG_AL,
    REG_BX, REG_BH, REG_BL,
    REG_CX, REG_CH, REG_CL,
    REG_DX, REG_DH, REG_DL,
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
