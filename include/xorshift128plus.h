#ifndef XORSHIFT128PLUS_H
#define XORSHIFT128PLUS_H

#include <stdint.h>// life is short, please use a C99-compliant compiler

/* Keys for scalar xorshift128. Must be non-zero
These are modified by xorshift128plus.
 */
struct xorshift128plus_key_s {
    uint64_t part1;
    uint64_t part2;
};

typedef struct xorshift128plus_key_s xorshift128plus_key_t;



/**
*
* You can create a new key like so...
*   xorshift128plus_key_t mykey;
*   xorshift128plus_init(324, 4444,&mykey);
*
* or directly if you prefer:
*  xorshift128plus_key_t mykey = {.part1=324,.part2=4444}
*
*  Then you can generate random numbers like so...
*      xorshift128plus(&mykey);
* If your application is threaded, each thread should have its own
* key.
*
*
* The seeds (key1 and key2) should be non-zero. You are responsible for
* checking that they are non-zero.
*
*/
static inline void xorshift128plus_init(uint64_t key1, uint64_t key2, xorshift128plus_key_t *key) {
  key->part1 = key1;
  key->part2 = key2;
}


/*
Return a new 64-bit random number
*/
uint64_t xorshift128plus(xorshift128plus_key_t * key);


/**
* equivalent to skipping 2^64 xorshift128plus() calls
* useful to generate a new key from an existing one (multi-threaded context).
*/
void xorshift128plus_jump(xorshift128plus_key_t * key);

/* Fisher-Yates shuffle, shuffling an array of 32-bit values, uses the provided key */
void  xorshift128plus_shuffle32(xorshift128plus_key_t * key, uint32_t *storage, uint32_t size);

/* Partial Fisher-Yates shuffle, shuffling  "size" 32-bit  values in "storage". You must provide the key for randomness. Applies shuffle to elements in [lower_index_inclusive, size). */
void  xorshift128plus_shuffle32_partial(xorshift128plus_key_t * key, uint32_t *storage, uint32_t size, uint32_t lower_index_inclusive);

#endif
