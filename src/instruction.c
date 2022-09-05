#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "helper.h"

#define INSTRUCTION_SIZE 5

Instruction decode_instruction(uint8_t opcode, uint16_t operand_1, uint16_t operand_2)
{
    Instruction inst = {
        .opcode = opcode,
        .operand_1.u = operand_1,
        .operand_2.u = operand_2
    };
    
    return inst;
}

Instruction* load_program(const char* path)
{
    FILE* file = fopen(path, "rb");
    INSIST(file != NULL, "file does not exist\n");    

    // get size of file
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // verify that the file size is valid
    INSIST((file_size - MAGIC_NUMBER_LENGTH) % INSTRUCTION_SIZE == 0, "invalid file\n");

    // turn file into array of bytes
    uint8_t* byte_array = malloc(file_size);
    INSIST(byte_array != NULL, "malloc failed\n");
    
    for (size_t i = 0; i < file_size; i++)
    {
        byte_array[i] = fgetc(file);
    }    
    fclose(file);
    
    // verify that the file starts with the magic number (D0D0FACE16)
    uint8_t magic_number[] = {0xd0, 0xd0, 0xfa, 0xce, 0x16};
    for (int i = 0; i < MAGIC_NUMBER_LENGTH; i++)
    {
        INSIST(magic_number[i] == byte_array[i], "no magic number :(\n");
    }
    
    // turn array of bytes into array of instruction
    int instruction_count = (file_size - MAGIC_NUMBER_LENGTH) / INSTRUCTION_SIZE;
    Instruction* program = malloc(instruction_count * sizeof(Instruction));
    INSIST(program != NULL, "malloc failed\n");
    
    for (int i = 0, j = MAGIC_NUMBER_LENGTH; i < instruction_count; i++, j += INSTRUCTION_SIZE)
    {
        Word op1 = {
            .bytes[0].u = byte_array[j + 1],
            .bytes[1].u = byte_array[j + 2]
        };
        Word op2 = {
            .bytes[0].u = byte_array[j + 3],
            .bytes[1].u = byte_array[j + 4]
        };        
        program[i] = decode_instruction(byte_array[j], op1.u, op2.u);
    }
    
    free(byte_array);
    return program;
}
