#ifndef HELPER_H
#define HELPER_H

#ifdef __MSVC__
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif

#define INSIST(condition, ...)                  \
    {                                           \
        if (!(condition))                       \
        {                                       \
            fprintf(stderr, __VA_ARGS__);       \
            exit(1);                            \
        }                                       \
    } void require_semicolon_()

#endif
