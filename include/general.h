#ifndef GENERAL_H
#define GENERAL_H

#include <stdint.h>

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
