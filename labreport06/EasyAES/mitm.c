#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "mitm.h"
#include "simple_aes/simple_aes.h"
#include "util.h"

/* 15732737 possible keys? */

bool keys_eq(uint8_t * k1, uint8_t * k2);

/* map a key to its encryption or decryption result */
struct KeyRes {
    uint8_t * key;
    uint8_t * res;
};

/* free a Key-Result mapping and its components */
void KeyRes_free(KeyRes * kr) {
    assert(kr != NULL);

    if (kr->key != NULL) {
        free(kr->key);
    }

    if (kr->res != NULL) {
        free(kr->res);
    }

    free(kr);
}

/* create and allocate memory for a new Key-Result mapping */
KeyRes * KeyRes_new(void) {
    KeyRes * kr = (KeyRes *) calloc(1, sizeof(KeyRes));
    kr->key = (uint8_t *) calloc(16, sizeof(uint8_t));
    kr->res = (uint8_t *) calloc(16, sizeof(uint8_t));

    CHECK_ALLOC(kr);
    CHECK_ALLOC(kr->key);
    CHECK_ALLOC(kr->res);

    return kr;
}

int main(void) {
    
    return EXIT_SUCCESS;
}
