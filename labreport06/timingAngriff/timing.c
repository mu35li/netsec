#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include "timing.h"

#define MAXLEN 20

struct Index2d {
    size_t x;
    size_t y;
};

const char * symbols = "abcdefghijklmnopqrstuvwxyz"
                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "äöü"
                       "0123456789"
                       "!@#$%^&*()-_=+[]{}|/?.,<>;:'\"\\`~"
                       "€´ ";

int main(void) {
    size_t len = strlen(symbols);
    clock_t ** times = (clock_t **) calloc(len, sizeof(clock_t *));
    for (uint32_t i = 0; i < len; i++) {
        times[i] = (clock_t *) calloc(MAXLEN, sizeof(clock_t));
    }

    clock_t start, stop;

    for (uint32_t k = 0; k < len; k++) {
        for (uint32_t i = 0; i < MAXLEN; i++) {
            char str[i + 1];
            for (uint32_t j = 0; j < i + 1; j++) {
                str[j] = symbols[k];
            }
            start = clock();
            for (uint64_t j = 0; j < 10000000; j++) {
                password_compare(str);
            }
            stop = clock();

            times[k][i] =  stop - start;
            assert(stop - start > 0);
        }
    }

    Index2d * max = (Index2d *) calloc(1, sizeof(Index2d));
    time_max(times, len, max);
    printf("length: %lu\n", max->y + 1);
    printf("first char: %c\n", symbols[max->x]);

    free(max);
    for (uint32_t i = 0; i < len; i++) {
        free(times[i]);
    }
    free(times);

    return EXIT_SUCCESS;
}

void time_max(clock_t ** arr, size_t len, Index2d * res) {
    clock_t max = 0;

    for (size_t i = 0; i < len; i++) {
        for (uint32_t j = 0; j < MAXLEN; j++) {
            if (arr[i][j] > max) {
                max = arr[i][j];
                res->x = i;
                res->y = j;
            }
        }
    }
}
