#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"

CPU* cpu_init()
{
    CPU* cpu = calloc(1, sizeof(CPU));
    return cpu;
}

void cpu_dump(CPU* cpu)
{
    printf(" a = %04x\n b = %04x\n c = %04x\n d = %04x\n r = %04x\nip = %04x\nsp = %04x\nstack = ",
           cpu->registers[REG_A].u,
           cpu->registers[REG_B].u,
           cpu->registers[REG_C].u,
           cpu->registers[REG_D].u,
           cpu->reg_r.u,
           cpu->reg_ip,
           cpu->reg_sp
    );

    for (int i = 0; i < MEMORY_STACK_SIZE - 1; i++)
    {
        printf("%04x\n        ", cpu->memory_stack[i].u);
    }
    printf("%04x\n", cpu->memory_stack[MEMORY_STACK_SIZE - 1].u);
}

void execute_instruction(CPU* cpu, Instruction inst)
{
    switch (inst.opcode)
    {
        case MOVL: {
            cpu->registers[inst.operand_1.u] = inst.operand_2;
        } break;
            
        case MOVR: {
            cpu->registers[inst.operand_1.u] = cpu->registers[inst.operand_2.u];
        } break;
            
        case ADDL: {
            cpu->registers[inst.operand_1.u].s += inst.operand_2.s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;
            
        case ADDR: {
            cpu->registers[inst.operand_1.u].s += cpu->registers[inst.operand_2.u].s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case SUBL: {
            cpu->registers[inst.operand_1.u].s -= inst.operand_2.s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case SUBR: {
            cpu->registers[inst.operand_1.u].s -= cpu->registers[inst.operand_2.u].s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case MULL: {
            cpu->registers[inst.operand_1.u].s *= inst.operand_2.s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case MULR: {
            cpu->registers[inst.operand_1.u].s *= cpu->registers[inst.operand_2.u].s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case DIVL: {
            cpu->registers[inst.operand_1.u].s /= inst.operand_2.s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case DIVR: {
            cpu->registers[inst.operand_1.u].s /= cpu->registers[inst.operand_2.u].s;
            cpu->reg_r = cpu->registers[inst.operand_1.u];
        } break;

        case CMPL: {
            cpu->reg_r.s = cpu->registers[inst.operand_1.u].s - inst.operand_2.s;
        } break;

        case CMPR: {
            cpu->reg_r.s = cpu->registers[inst.operand_1.u].s - cpu->registers[inst.operand_2.u].s;
        } break;
            
        case JMP: {
            cpu->reg_ip = inst.operand_1.u - 1;
        } break;

        case JZ: {
            if (cpu->reg_r.s == 0)
                cpu->reg_ip = inst.operand_1.u - 1;
        } break;

        case JNZ: {
            if (cpu->reg_r.s != 0)
                cpu->reg_ip = inst.operand_1.u - 1;
        } break;

        case JL:
            if (cpu->reg_r.s < 0)
                cpu->reg_ip = inst.operand_1.u - 1;
            break;

        case JLE: {
            if (cpu->reg_r.s <= 0)
                cpu->reg_ip = inst.operand_1.u - 1;
        } break;

        case JG: {
            if (cpu->reg_r.s > 0)
                cpu->reg_ip = inst.operand_1.u - 1;
        } break;
            
        case JGE: {
            if (cpu->reg_r.s >= 0)
                cpu->reg_ip = inst.operand_1.u - 1;
        } break;

        case PUSHL: {
            cpu->reg_sp++;
            uint16_t sp = cpu->reg_sp;
            cpu->memory_stack[sp] = inst.operand_1;
        } break;

        case PUSHR: {
            cpu->reg_sp++;
            uint16_t sp = cpu->reg_sp;
            cpu->memory_stack[sp] = cpu->registers[inst.operand_1.u];
        } break;

        case POPR: {
            uint16_t sp = cpu->reg_sp;                       
            cpu->registers[inst.operand_1.u] = cpu->memory_stack[sp];
            cpu->reg_sp--;
        } break;
            
        case HLT: {
            cpu->should_halt = true;
        } break;
            
        default: {
            printf("Unimplemented!!!!\n");
        }
    }
    
    cpu->reg_ip++;
}
