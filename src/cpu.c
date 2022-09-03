#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"

#define GET_REGISTER_INDEX(reg) ((reg) / 3)

FORCE_INLINE
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

FORCE_INLINE
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
    printf("ax = %02x %02x\nbx = %02x %02x\ncx = %02x %02x\ndx = %02x %02x\n\nrx = %04x\nip = %04x\nsp = %04x\nstack = ",
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
    
    /* for (int i = 0; i < MEMORY_STACK_SIZE - 1; i++) */
    /* { */
    /*     printf("%04x\n        ", cpu->memory_stack[i].u); */
    /* } */
    /* printf("%04x\n", cpu->memory_stack[MEMORY_STACK_SIZE - 1].u); */
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
