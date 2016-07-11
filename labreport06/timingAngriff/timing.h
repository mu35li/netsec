#pragma once

#include <stdbool.h>
#include <time.h>
#include <stdint.h>

extern bool password_compare(const char * password);
uint32_t time_max(const clock_t * arr, uint32_t len);
