#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "helper.h"

#define GET_REGISTER_INDEX(reg) ((reg) / 3)

static FORCE_INLINE
Word get_register(CPU* cpu, Register reg)
{       
    int index = GET_REGISTER_INDEX(reg);
    
    Word result;
    switch (reg)
    {     
        case REG_AX: case REG_BX: case REG_CX: case REG_DX:
        {
            result = cpu->registers[index];
        } break;
            
        case REG_AH: case REG_BH: case REG_CH: case REG_DH: {
            result = (Word){
                .u = (cpu->registers[index].u & 0xFF00) >> 8
            };
        } break;
            
        case REG_AL: case REG_BL: case REG_CL: case REG_DL: {
            result = (Word){
                .u = cpu->registers[index].u & 0x00FF
            };
        } break;

        default: {
            fprintf(stderr, "invalid register\n");
            exit(1);
        } break;   
    }

    return result;
}

static FORCE_INLINE
void set_register(CPU* cpu, Register reg, Word value)
{
    int index = GET_REGISTER_INDEX(reg);
    
    switch (reg)
    {
        case REG_AX: case REG_BX: case REG_CX: case REG_DX: {
            cpu->registers[index] = value;
        } break;
            
        case REG_AH: case REG_BH: case REG_CH: case REG_DH: {
            cpu->registers[index].u = (value.u << 8) | cpu->registers[index].u;
        } break;
            
        case REG_AL: case REG_BL: case REG_CL: case REG_DL: {
            cpu->registers[index].u = (value.u & 0x00FF) | cpu->registers[index].u;
        } break;

        default: {
            fprintf(stderr, "invalid register\n");
            exit(1);
        } break;            
    }
}

CPU* cpu_init()
{
    CPU* cpu = calloc(1, sizeof(CPU));
    return cpu;
}

void cpu_dump(CPU* cpu)
{
    printf("ax = %02x %02x\nbx = %02x %02x\ncx = %02x %02x\ndx = %02x %02x\n\n r = %04x\nip = %04x\nsp = %04x\n\nstack = ",
           get_register(cpu, REG_AH).u,
           get_register(cpu, REG_AL).u,
           get_register(cpu, REG_BH).u,
           get_register(cpu, REG_BL).u,           
           get_register(cpu, REG_CH).u,
           get_register(cpu, REG_CL).u,
           get_register(cpu, REG_DH).u,
           get_register(cpu, REG_DL).u,
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
            set_register(cpu, inst.operand_1.u, inst.operand_2);
        } break;

        case MOVR: {
            Word new_value = get_register(cpu, inst.operand_2.u);
            set_register(cpu, inst.operand_1.u, new_value);
        } break;

        case ADDL: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s + inst.operand_2.s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case ADDR: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s + get_register(cpu, inst.operand_2.u).s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case SUBL: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s - inst.operand_2.s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case SUBR: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s - get_register(cpu, inst.operand_2.u).s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case MULL: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s * inst.operand_2.s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case MULR: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s * get_register(cpu, inst.operand_2.u).s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case DIVL: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s / inst.operand_2.s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case DIVR: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s / get_register(cpu, inst.operand_2.u).s
            };
            set_register(cpu, inst.operand_1.u, result);
            cpu->reg_r = result;
        } break;

        case CMPL: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s - inst.operand_2.s
            };
            cpu->reg_r = result;
        } break;

        case CMPR: {
            Word result = {
                .s = get_register(cpu, inst.operand_1.u).s - get_register(cpu, inst.operand_2.u).s
            };
            cpu->reg_r = result;
        } break;

        case JMP: {
            cpu->reg_ip = inst.operand_1.u;
        } break;

        case JZ: {
            if (cpu->reg_r.s == 0) cpu->reg_ip = inst.operand_1.u;
        } break;

        case JNZ: {
            if (cpu->reg_r.s != 0) cpu->reg_ip = inst.operand_1.u;
        } break;
                        
        case JL: {
            if (cpu->reg_r.s < 0) cpu->reg_ip = inst.operand_1.u;
        } break;

        case JLE: {
            if (cpu->reg_r.s <= 0) cpu->reg_ip = inst.operand_1.u;
        } break;

        case JG: {
            if (cpu->reg_r.s > 0) cpu->reg_ip = inst.operand_1.u;
        } break;

        case JGE: {
            if (cpu->reg_r.s >= 0) cpu->reg_ip = inst.operand_1.u;
        } break;

        case CALL: {
            cpu->reg_rsp++;
            cpu->return_stack[cpu->reg_rsp] = cpu->reg_ip;
            cpu->reg_ip = inst.operand_1.u;            
        } break;

        case RET: {
            cpu->reg_ip = cpu->return_stack[cpu->reg_rsp];
            cpu->reg_rsp--;
        } break;

        case PUSHL: {
            cpu->reg_sp++;
            cpu->memory_stack[cpu->reg_sp] = inst.operand_1;
        } break;

        case PUSHR: {
            cpu->reg_sp++;
            cpu->memory_stack[cpu->reg_sp] = get_register(cpu, inst.operand_1.u);
        } break;

        case POPR: {
            Word memory_stack_top = cpu->memory_stack[cpu->reg_sp];
            set_register(cpu, inst.operand_1.u, memory_stack_top);
            cpu->reg_sp--;
        } break;
        
        case HLT: {
            cpu->should_halt = true;
            puts("program has successfully terminated.");
        } break;
            
        default: {
            printf("Unimplemented!!!!\n");
        }
    }
    
    cpu->reg_ip++;
}
