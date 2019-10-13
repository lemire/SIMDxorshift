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
 * computer two random number in [0,bound1) and [0,bound2)
 * has a slight bias, but that's probably the last of your concerns given
 * that xorshift128plus is not perfect to begin with.
 */
static void xorshift128plus_bounded_two_by_two(xorshift128plus_key_t * key,
		uint32_t bound1, uint32_t bound2, uint32_t* bounded1, uint32_t * bounded2) {
	uint64_t rand = xorshift128plus(key);
	* bounded1 = ((rand & UINT64_C(0xFFFFFFFF)) * bound1) >> 32;
	* bounded2 = ((rand >> 32) * bound2) >> 32;
}

static uint32_t xorshift128plus_bounded(xorshift128plus_key_t * key, uint32_t bound) {
	uint64_t rand = xorshift128plus(key);
	return ((rand & UINT64_C(0xFFFFFFFF)) * bound ) >> 32;
}


/* Fisher-Yates shuffle, shuffling an array of integers, uses the provided key */
void  xorshift128plus_shuffle32(xorshift128plus_key_t * key, uint32_t *storage, uint32_t size) {
    xorshift128plus_shuffle32_partial(key, storage, size, 1);
}

void  xorshift128plus_shuffle32_partial(xorshift128plus_key_t * key, uint32_t *storage, uint32_t size, uint32_t lower_index_inclusive) {
    uint32_t i;
    uint32_t nextpos1, nextpos2;
    if (lower_index_inclusive==0) lower_index_inclusive=1;
    for (i=size; i>(lower_index_inclusive+1); i-=2) {
    	xorshift128plus_bounded_two_by_two(key,i,i-1,&nextpos1,&nextpos2);
        const uint32_t tmp1 = storage[i-1];// likely in cache
        const uint32_t val1 = storage[nextpos1]; // could be costly
        storage[i - 1] = val1;
        storage[nextpos1] = tmp1; // you might have to read this store later

        uint32_t tmp2 = storage[i-2];// likely in cache
        uint32_t val2 = storage[nextpos2]; // could be costly
        storage[i - 2] = val2;
        storage[nextpos2] = tmp2; // you might have to read this store later

    }
    if(i>lower_index_inclusive) {
    	const uint32_t nextpos = xorshift128plus_bounded(key,i);
    	const uint32_t tmp = storage[i-1];// likely in cache
    	const uint32_t val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }

}
