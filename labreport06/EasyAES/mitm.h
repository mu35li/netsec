#pragma once

#include <stdint.h>

#define KEYLEN 16
#define BYTE_MAX 255

/* map a key to its encryption or decryption result */
typedef struct KeyRes KeyRes;

/* storage for multiple KeyRes structs */
typedef struct KeyResStore KeyResStore;

/* free a Key-Result mapping and its components */
void KeyRes_free(KeyRes * kr);

/* create and allocate memory for a new Key-Result mapping */
KeyRes * KeyRes_new(void);

/* free a key-result-store */
void KeyResStore_free(KeyResStore * krs);

/* create and allocate some memory for a new key-result-store */
KeyResStore * KeyResStore_new(void);

/* resize a key-result-store to its current capacity plus the specified number n */
void KeyResStore_resize(KeyResStore * krs, uint64_t n);

/* add a key-result mapping to a key-result-store, resizing the latter if necessary */
void KeyResStore_add(KeyResStore * krs, KeyRes * kr);
