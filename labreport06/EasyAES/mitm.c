#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <omp.h>
#include "mitm.h"
#include "simple_aes/simple_aes.h"
#include "util.h"

/* 7807081 possible keys */

const uint8_t plaintext[16] = {'V','e','r','s','c','h','l','u','e','s','s','e','l','u','n','g'};
const uint8_t ciphertext[16] = {
    0xbe, 0x39, 0x3d, 0x39,
    0xca, 0x4e, 0x18, 0xf4,
    0x1f, 0xa9, 0xd8, 0x8a,
    0x9d, 0x47, 0xa5, 0x74
};

/*const uint8_t plaintext[16] = {*/
    /*0x00, 0x00, 0x00, 0x00,*/
    /*0x00, 0x00, 0x00, 0x00,*/
    /*0x00, 0x00, 0x00, 0x00,*/
    /*0x00, 0x00, 0x00, 0x00*/
/*};*/

/*const uint8_t ciphertext[16] = {*/
    /*0x56, 0x6a, 0x77, 0xa1,*/
    /*0x68, 0x36, 0x7d, 0xf8,*/
    /*0x53, 0x46, 0x4c, 0x47,*/
    /*0x02, 0xe4, 0x87, 0x4a*/
/*};*/

/* map a key to its encryption or decryption result */
struct KeyRes {
    uint8_t * key;
    uint8_t * res;
};

/* storage for multiple KeyRes structs */
struct KeyResStore {
    KeyRes ** items;
    uint64_t size;
    uint64_t capacity;
};

int main(void) {
    /* pseudo-code:
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
     */

    /* init our key-result-store */
    KeyResStore ** store = (KeyResStore **) calloc(256, sizeof(KeyResStore));
    CHECK_ALLOC(store);

    for (uint32_t i = 0; i < 256; i++) {
        store[i] = KeyResStore_new();
    }

    /* populate it with mappings of all legal keys to their encryption results */
    KeyResStore_populate(store);
    fprintf(stderr, "populated key-result-store\n");

    /* do our attack by comparing all stored encryption results
     * with all decryption results until we have a match.
     * Result will be printed to stdout */
    fprintf(stderr, "starting meet-in-the-middle attack...\n");
    meet_in_the_middle(store);
    fprintf(stderr, "done!\n");

    /* free our key-result-store; this also frees all key-result-maps stored */
    for (uint32_t i = 0; i < 256; i++) {
        KeyResStore_free(store[i]);
    }

    /* we were successful! */
    return EXIT_SUCCESS;
}

/* free a Key-Result mapping and its components */
void KeyRes_free(KeyRes * kr) {
    assert(kr != NULL);

    free(kr->key);
    free(kr->res);
    free(kr);
    DBPRINT("free KeyRes\n");
}

/* create and allocate memory for a new Key-Result mapping */
KeyRes * KeyRes_new(void) {
    KeyRes * kr = (KeyRes *) calloc(1, sizeof(KeyRes));
    CHECK_ALLOC(kr);

    kr->key = (uint8_t *) calloc(16, sizeof(uint8_t));
    CHECK_ALLOC(kr->key);

    kr->res = (uint8_t *) calloc(16, sizeof(uint8_t));
    CHECK_ALLOC(kr->res);

    DBPRINT("create new KeyRes\n");
    return kr;
}

void KeyResStore_free(KeyResStore * krs) {
    assert(krs != NULL);

    for (uint64_t i = 0; i < krs->size; i++) {
        KeyRes_free(krs->items[i]);
    }

    free(krs->items);
    free(krs);
    DBPRINT("free KeyResStore\n");
}

KeyResStore * KeyResStore_new(void) {
    KeyResStore * krs = (KeyResStore *) calloc(1, sizeof(KeyResStore));

    CHECK_ALLOC(krs);

    krs->items = (KeyRes **) calloc(100, sizeof(KeyRes **));

    CHECK_ALLOC(krs->items);

    krs->size = 0;
    krs->capacity = 100;

    DBPRINT("create new KeyResStore\n");
    return krs;
}

void KeyResStore_resize(KeyResStore * krs, uint64_t n) {
    assert(krs != NULL);

    KeyRes ** temp = (KeyRes **) realloc(krs->items, (krs->capacity + n) * sizeof(KeyRes **));

    CHECK_ALLOC(temp);

    krs->items = temp;
    krs->capacity += n;
    DBPRINT("resize KeyResStore\n");
}

void KeyResStore_add(KeyResStore * krs, KeyRes * kr) {
    assert(krs != NULL);

    /* if key-result-store is full, give it capacity for 1000 more */
    if (krs->size == krs->capacity) {
        KeyResStore_resize(krs, 1000);
    }

    krs->items[krs->size] = kr;
    krs->size++;
    DBPRINT("add KeyRes to KeyResStore\n");
}

void KeyResStore_populate(KeyResStore ** krs) {
    uint8_t byte1_pos = 0;
    uint8_t byte1_val = 0x01;
    uint64_t count = 1;

    /* add key with only null-bytes */
    uint8_t * nullpt = (uint8_t *) calloc(16, sizeof(uint8_t));
    CHECK_ALLOC(nullpt);

    KeyRes * nullkr = KeyRes_new();
    for (uint8_t i = 0; i < 16; i++) {
        nullpt[i] = plaintext[i];
    }

    aes128_encrypt(nullpt, nullkr->key);

    for (uint8_t i = 0; i < 16; i++) {
        nullkr->res[i] = nullpt[i];
    }

    KeyResStore_add(krs[nullpt[0]], nullkr);

    free(nullpt);

    /* populate with 1-non-null-byte keys */
    while (true) {
        KeyRes * kr = KeyRes_new();
        kr->key[byte1_pos] = byte1_val;

        uint8_t * pt = (uint8_t *) calloc(16, sizeof(uint8_t));
        CHECK_ALLOC(pt);

        for (uint8_t i = 0; i < 16; i++) {
            pt[i] = plaintext[i];
        }

        aes128_encrypt(pt, kr->key);

        for (uint8_t i = 0; i < 16; i++) {
            kr->res[i] = pt[i];
        }

        KeyResStore_add(krs[pt[0]], kr);
        count++;
        free(pt);

        if (byte1_pos == 15 && byte1_val == 0xff) {
            break;
        }

        if (byte1_val == 0xff) {
            byte1_pos++;
            byte1_val = 0x01;
        }
        else {
            byte1_val++;
        }
    }

    byte1_pos = 0;
    byte1_val = 0x01;
    uint8_t byte2_pos = 1;
    uint8_t byte2_val = 0x01;

    /* populate with 2-non-null-byte keys */
    while (true) {
        KeyRes * kr = KeyRes_new();
        kr->key[byte1_pos] = byte1_val;
        kr->key[byte2_pos] = byte2_val;

        uint8_t * pt = (uint8_t *) calloc(16, sizeof(uint8_t));
        CHECK_ALLOC(pt);

        for (uint8_t i = 0; i < 16; i++) {
            pt[i] = plaintext[i];
        }

        aes128_encrypt(pt, kr->key);

        for (uint8_t i = 0; i < 16; i++) {
            kr->res[i] = pt[i];
        }

        KeyResStore_add(krs[pt[0]], kr);
        count++;
        free(pt);

        if (byte1_pos == 14 && byte2_pos == 15 \
            && byte1_val == 0xff && byte2_val == 0xff) {
            break;
        }

        if (byte1_val == 0xff && byte2_val == 0xff) {
            byte1_val = 0x01;
            byte2_val = 0x01;
            byte1_pos++;

            if (byte1_pos == byte2_pos) {
                byte2_pos++;
                byte1_pos = 0;
            }

            continue;
        }

        if (byte1_val == 0xff) {
            byte1_val = 0x01;
            byte2_val++;
        }
        else {
            byte1_val++;
        }
    }

    fprintf(stderr, "nr of keys: %lu\n", count);
}

void meet_in_the_middle(KeyResStore ** krs) {
    uint8_t byte1_pos = 0;
    uint8_t byte1_val = 0x01;
    //uint64_t counter = 0;
    bool done = false;

    /* try key with only null-bytes */
    uint8_t * nullkey = (uint8_t *) calloc(16, sizeof(uint8_t));
    CHECK_ALLOC(nullkey);
    uint8_t * nullct = (uint8_t *) calloc(16, sizeof(uint8_t));
    CHECK_ALLOC(nullct);

    for (uint8_t i = 0; i < 16; i++) {
        nullct[i] = ciphertext[i];
    }

    aes128_decrypt(nullct, nullkey);

    for (uint64_t i = 0; i < krs[nullct[0]]->size; i++) {
        if (txt_eq(krs[nullct[0]]->items[i]->res, nullct)) {
            printf("MATCH FOUND!\n");
            printf("Key 1: ");
            for (uint8_t j = 0; j < 15; j++) {
                printf("0x%02x, ", krs[nullct[0]]->items[i]->key[j]);
            }
            printf("0x%02x\n", krs[nullct[0]]->items[i]->key[15]);

            printf("Key 2: ");
            for (uint8_t j = 0; j < 15; j++) {
                printf("0x%02x, ", nullkey[j]);
            }
            printf("0x%02x\n", nullkey[15]);
            free(nullct);
            free(nullkey);
            return;
        }
    }

    free(nullct);
    free(nullkey);

    /* decrypt with 1-non-null-byte keys */
    while (!done) {
        uint8_t * key = (uint8_t *) calloc(16, sizeof(uint8_t));
        CHECK_ALLOC(key);

        key[byte1_pos] = byte1_val;

        uint8_t * ct = (uint8_t *) calloc(16, sizeof(uint8_t));
        CHECK_ALLOC(ct);

        for (uint8_t i = 0; i < 16; i++) {
            ct[i] = ciphertext[i];
        }

        aes128_decrypt(ct, key);

        #pragma omp parallel for shared(done)
        for (uint64_t i = 0; i < krs[ct[0]]->size; i++) {
            /*counter++;
            if (counter % 100000 == 0) {
                fprintf(stderr, "%lu iterations completed.\n", counter);
            }*/

            if (done) continue;

            if (txt_eq(krs[ct[0]]->items[i]->res, ct)) {
                printf("MATCH FOUND!\n");
                printf("Key 1: ");
                for (uint8_t j = 0; j < 15; j++) {
                    printf("0x%02x, ", krs[ct[0]]->items[i]->key[j]);
                }
                printf("0x%02x\n", krs[ct[0]]->items[i]->key[15]);

                printf("Key 2: ");
                for (uint8_t j = 0; j < 15; j++) {
                    printf("0x%02x, ", key[j]);
                }
                printf("0x%02x\n", key[15]);

                done = true;
            }
        }

        /* fprintf(stderr, "finish checking key: ");
        for (uint8_t i = 0; i < 15; i++) {
            fprintf(stderr, "0x%02x, ", key[i]);
        }
        fprintf(stderr, "0x%02x\n", key[15]); */

        free(key);
        free(ct);

        if (byte1_pos == 15 && byte1_val == 0xff) {
            break;
        }

        if (byte1_val == 0xff) {
            byte1_pos++;
            byte1_val = 0x01;
        }
        else {
            byte1_val++;
        }
    }

    fprintf(stderr, "1-non-null-byte keys done, no match yet.\n");

    byte1_pos = 0;
    byte1_val = 0x01;
    uint8_t byte2_pos = 1;
    uint8_t byte2_val = 0x01;

    /* populate with 2-non-null-byte keys */
    while (!done) {
        uint8_t * key = (uint8_t *) calloc(16, sizeof(uint8_t));
        CHECK_ALLOC(key);

        key[byte1_pos] = byte1_val;
        key[byte2_pos] = byte2_val;

        uint8_t * ct = (uint8_t *) calloc(16, sizeof(uint8_t));
        CHECK_ALLOC(ct);

        for (uint8_t i = 0; i < 16; i++) {
            ct[i] = ciphertext[i];
        }

        aes128_decrypt(ct, key);

        #pragma omp parallel for shared(done)
        for (uint64_t i = 0; i < krs[ct[0]]->size; i++) {
            /*counter++;
            if (counter % 100000 == 0) {
                fprintf(stderr, "%lu iterations completed.\n", counter);
            }*/

            if (done) continue;

            if (txt_eq(krs[ct[0]]->items[i]->res, ct)) {
                printf("MATCH FOUND!\n");
                printf("Key 1: ");
                for (uint8_t j = 0; j < 15; j++) {
                    printf("0x%02x, ", krs[ct[0]]->items[i]->key[j]);
                }
                printf("0x%02x\n", krs[ct[0]]->items[i]->key[15]);

                printf("Key 2: ");
                for (uint8_t j = 0; j < 15; j++) {
                    printf("0x%02x, ", key[j]);
                }
                printf("0x%02x\n", key[15]);

                done = true;
            }
        }

        /* fprintf(stderr, "finish checking key: ");
        for (uint8_t i = 0; i < 15; i++) {
            fprintf(stderr, "0x%02x, ", key[i]);
        }
        fprintf(stderr, "0x%02x\n", key[15]); */

        free(key);
        free(ct);

        if (byte1_pos == 14 && byte2_pos == 15\
            && byte1_val == 0xff && byte2_val == 0xff) {
            break;
        }

        if (byte1_val == 0xff && byte2_val == 0xff) {
            byte1_val = 0x01;
            byte2_val = 0x01;
            byte1_pos++;

            if (byte1_pos == byte2_pos) {
                byte2_pos++;
                byte1_pos = 0;
            }

            continue;
        }

        if (byte1_val == 0xff) {
            byte1_val = 0x01;
            byte2_val++;
        }
        else {
            byte1_val++;
        }
    }

    printf("No match found!\n");
}

bool txt_eq(uint8_t * t1, uint8_t * t2) {
    for (uint8_t i = 0; i < 16; i++) {
        if (t1[i] != t2[i]) {
            return false;
        }
    }
    return true;
}
