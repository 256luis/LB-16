#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include "general.h"

typedef enum Opcode
{
    MOVL, MOVR,
    ADDL, ADDR,
    SUBL, SUBR,
    MULL, MULR,
    DIVL, DIVR,
    CMPL, CMPR,
    JMP,
    JZ,   JNZ,
    JL,   JLE,
    JG,   JGE,
    CALL, RET,
    PUSHL, PUSHR,
    POPR,
    HLT
} Opcode;

typedef struct Instruction
{
    Opcode opcode;
    Word operand_1;
    Word operand_2;
} Instruction;

Instruction decode_instruction(uint8_t opcode, uint16_t operand_1, uint16_t operand_2);
Instruction* load_program(const char* path);

#endif
