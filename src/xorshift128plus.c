#include "xorshift128plus.h"

uint64_t xorshift128plus(xorshift128plus_key_t * key) {
    uint64_t s1 = key->part1;
    const uint64_t s0 = key->part2;
    key->part1 = s0;
    s1 ^= s1 << 23; // a
    key->part2 = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
    return key->part2 + s0;
}


void xorshift128plus_jump(xorshift128plus_key_t * key) {
    static const uint64_t JUMP[] = { 0x8a5cd789635d2dff, 0x121fd2155c472f96 };
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    for(unsigned int i = 0; i < sizeof (JUMP) / sizeof (*JUMP); i++)
        for(int b = 0; b < 64; b++) {
            if (JUMP[i] & 1ULL << b) {
                s0 ^= key->part1;
                s1 ^= key->part2;
            }
            xorshift128plus(key);
        }

    key->part1 = s0;
    key->part2 = s1;
}


/**
 * return a random number in [0,bound)
 * has a slight bias, but that's probably the last of your concerns given
 * that xorshift128plus is not perfect to begin with.
 */
static uint32_t xorshift128plus_bounded(xorshift128plus_key_t * key, uint32_t bound) {
	return (uint32_t)(((uint64_t)xorshift128plus(key) * (uint64_t)bound) >> 32);
}

/* Fisher-Yates shuffle, shuffling an array of integers, uses the provided key */
void  xorshift128plus_shuffle32(xorshift128plus_key_t * key, uint32_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = xorshift128plus_bounded(key,i);
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}
