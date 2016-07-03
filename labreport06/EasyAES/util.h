#pragma once
#include <stdio.h>
#include <stdlib.h>

#define CHECK_ALLOC(ptr) \
do { \
    if (ptr == NULL) { \
        fprintf(stderr, "memory allocation failure in %s:%d\n", \
                __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
    } \
} while (0)

#ifdef DEBUG
#   define DBPRINT(s) do {fprintf(stderr, s);} while(0)
#else
#   define DBPRINT(s)
#endif
