#ifndef GENERAL_H
#define GENERAL_H

#include <stdint.h>

// temporarily here. should be in instruction.c only
#define MAGIC_NUMBER_LENGTH 5

typedef union Byte
{
    int8_t s;
    uint8_t u;   
} Byte;

typedef union Word
{
    int16_t s;
    uint16_t u;
    Byte bytes[2];
} Word;

#endif
