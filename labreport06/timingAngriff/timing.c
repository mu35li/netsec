#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include "timing.h"

#define MAXLEN 24

const char * symbols = "abcdefghijklmnopqrstuvwxyz"
                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "äöü"
                       "0123456789"
                       "!@#$%^&*()-_=+[]{}|/?.,<>;:'\"\\`~"
                       "€´ ";

int main(void) {
    /*size_t len = strlen(symbols);*/
    clock_t times[MAXLEN];
    clock_t start, stop;

    for (uint32_t i = 0; i < MAXLEN; i++) {
        char str[i + 1];
        for (uint32_t j = 0; j < i + 1; j++) {
            str[j] = 'a';
        }
        start = clock();
        for (uint64_t j = 0; j < 1000000000; j++) {
            password_compare(str);
        }
        stop = clock();

        times[i] =  stop - start;
        assert(stop - start > 0);
    }

    uint32_t max = time_max(times, MAXLEN) + 1;
    printf("length: %u\n", max);
    return EXIT_SUCCESS;
}

uint32_t time_max(const clock_t * arr, uint32_t len) {
    clock_t max = 0;
    uint32_t res = 0;
    for (uint32_t i = 0; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
            res = i;
        }
    }

    return res;
}
