#pragma once

#define KEYLEN 16
#define BYTE_MAX 255

/* map a key to its encryption or decryption result */
typedef struct KeyRes KeyRes;

/* free a Key-Result mapping and its components */
void KeyRes_free(KeyRes * kr);

/* create and allocate memory for a new Key-Result mapping */
KeyRes * KeyRes_new(void);
