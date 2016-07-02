#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
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

/* storage for multiple KeyRes structs */
struct KeyResStore {
    KeyRes * items;
    uint64_t size;
    uint64_t capacity;
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

    CHECK_ALLOC(kr);

    kr->key = (uint8_t *) calloc(16, sizeof(uint8_t));
    kr->res = (uint8_t *) calloc(16, sizeof(uint8_t));

    CHECK_ALLOC(kr->key);
    CHECK_ALLOC(kr->res);

    return kr;
}

void KeyResStore_free(KeyResStore * krs) {
    assert(krs != NULL);

    if (krs->items != NULL) {
        for (uint64_t i = 0; i < krs->size; i++) {
            KeyRes_free(&krs->items[0]);
        }
    }

    free(krs);
}

KeyResStore * KeyResStore_new(void) {
    KeyResStore * krs = (KeyResStore *) calloc(1, sizeof(KeyResStore));

    CHECK_ALLOC(krs);

    krs->items = (KeyRes *) calloc(100, sizeof(KeyRes));
    krs->size = 0;
    krs->capacity = 100;

    return krs;
}

void KeyResStore_resize(KeyResStore * krs, uint64_t n) {
    assert(krs != NULL);

    krs->items = (KeyRes *) realloc(krs->items, krs->capacity + n);
    CHECK_ALLOC(krs->items);
}

void KeyResStore_add(KeyResStore * krs, KeyRes * kr) {
    assert(krs != NULL);

    /* if key-result-store is full, give it capacity for a 100 more */
    if (krs->size == krs->capacity) {
        KeyResStore_resize(krs, krs->capacity + 100);
    }

    krs->items[krs->size] = *kr;
    krs->size += 1;
}

int main(void) {
    /*
     * pseudo-code:
     * for key1 in keys:
     *     result1 = encrypt(plaintext, key1)
     *     kr = KeyRes_new()
     *     kr->key = key1
     *     kr->res = result1
     *     store(kr)
     * for key2 in keys:
     *     result2 = decrypt(ciphertext, key2)
     *     if result2 == retrieve(result1):
     *         print(key1)
     *         print(key2)
     *         return EXIT_SUCCESS;
     *  return EXIT_FAILURE;
     *
     */

    /* initialize a kr-store, a kr map and add the latter to the former,
     * just to see if it works...
     */
    KeyResStore * kr_store = KeyResStore_new();
    KeyRes * lol = KeyRes_new();
    KeyResStore_add(kr_store, lol);

    KeyResStore_free(kr_store);
    return EXIT_SUCCESS;
}
