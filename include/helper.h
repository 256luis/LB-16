#ifndef HELPER_H
#define HELPER_H

#define INSIST(condition, ...)                  \
    {                                           \
        if (!(condition))                       \
        {                                       \
            fprintf(stderr, __VA_ARGS__);       \
            exit(1);                            \
        }                                       \
    } void require_semicolon_()

#endif
