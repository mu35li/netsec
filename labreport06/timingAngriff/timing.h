#pragma once

#include <stdbool.h>
#include <time.h>
#include <stdint.h>

typedef struct Index2d Index2d;

extern bool password_compare(const char * password);
void time_max(clock_t ** arr, size_t len, Index2d * res);
