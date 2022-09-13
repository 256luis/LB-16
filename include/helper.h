#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __MSVC__
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif // __MSVC__

#define INSIST(condition, ...)                  \
    {                                           \
        if (!(condition))                       \
        {                                       \
            fprintf(stderr, __VA_ARGS__);       \
            exit(1);                            \
        }                                       \
    } void require_semicolon_()

size_t get_file_size(FILE* file);
uint8_t* file_to_array(FILE* file);
char* file_to_string(FILE* file);

#ifdef HELPER_IMPLEMENTATION

size_t get_file_size(FILE* file)
{
    int current_position = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, current_position, SEEK_SET);

    return file_size;
}

uint8_t* file_to_array(FILE* file)
{
    size_t size = get_file_size(file);
    uint8_t* byte_array = malloc(size);
    if (byte_array == NULL) return NULL;
    
    for (size_t i = 0; i < size; i++)
    {
        byte_array[i] = fgetc(file);
    }

    return byte_array;
}

char* file_to_string(FILE* file)
{
    int size = get_file_size(file);
    uint8_t* byte_array = file_to_array(file);
    if (byte_array == NULL) return NULL;
    
    byte_array[size - 1] = 0;
    return (char*)byte_array;
}

#endif // HELPER_IMPLEMENTATION
#endif // HELPER_H
