#ifndef SIMDXORSHIFT128PLUS_H
#define SIMDXORSHIFT128PLUS_H

#include <stdint.h>// life is short, please use a C99-compliant compiler

#if defined(_MSC_VER)
     /* Microsoft C/C++-compatible compiler */
     #include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
     /* GCC-compatible compiler, targeting x86/x86-64 */
     #include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__)
     /* GCC-compatible compiler, targeting ARM with NEON */
     #include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__)
     /* GCC-compatible compiler, targeting ARM with WMMX */
     #include <mmintrin.h>
#elif (defined(__GNUC__) || defined(__xlC__)) && (defined(__VEC__) || defined(__ALTIVEC__))
     /* XLC or GCC-compatible compiler, targeting PowerPC with VMX/VSX */
     #include <altivec.h>
#elif defined(__GNUC__) && defined(__SPE__)
     /* GCC-compatible compiler, targeting PowerPC with SPE */
     #include <spe.h>
#endif


/* Keys for scalar xorshift128. Must be non-zero
These are modified by xorshift128plus.
 */
struct avx_xorshift128plus_key_s {
    __m256i part1;
    __m256i part2;
};

typedef struct avx_xorshift128plus_key_s avx_xorshift128plus_key_t;



/**
* You can create a new key like so...
*  avx_xorshift128plus_key_t mykey;
*  avx_xorshift128plus_init(324,4444,&mykey);
*
* This feeds the two integers (324 and 4444) as seeds to the random
* number generator.
*
*  Then you can generate random numbers like so...
*      avx_xorshift128plus(&mykey);
* If your application is threaded, each thread should have its own
* key.
*
*
* The seeds (key1 and key2) should be non-zero. You are responsible for
* checking that they are non-zero.
*/
void avx_xorshift128plus_init(uint64_t key1, uint64_t key2, avx_xorshift128plus_key_t *key);

/*
Return a 256-bit random "number"
*/
__m256i avx_xorshift128plus( avx_xorshift128plus_key_t *key);

/**
* equivalent to skipping 2^64 avx_xorshift128plus() calls
* useful to generate a new key from an existing one (multi-threaded context).
*/
void avx_xorshift128plus_jump(avx_xorshift128plus_key_t * key);

/**
 * Fisher-Yates shuffle, shuffling  "size" 32-bit  values in "storage". You must provide the key for
 * randomness.
 */
void  avx_xorshift128plus_shuffle32(avx_xorshift128plus_key_t *key, uint32_t *storage, uint32_t size);

/**
 * Partial Fisher-Yates shuffle, shuffling  "size" 32-bit  values in "storage". You must provide the key for
 * randomness. Stops shuffling after finalizing all elements in [lower_index_inclusive, size).
 */
void  avx_xorshift128plus_shuffle32_partial(avx_xorshift128plus_key_t *key, uint32_t *storage, uint32_t size, uint32_t lower_index_inclusive);

#if defined(__AVX512F__) 

struct avx512_xorshift128plus_key_s {
    __m512i part1;
    __m512i part2;
};

typedef struct avx512_xorshift128plus_key_s avx512_xorshift128plus_key_t;



/**
* You can create a new key like so...
*  avx_xorshift128plus_key_t mykey;
*  avx_xorshift128plus_init(324,4444,&mykey);
*
* This feeds the two integers (324 and 4444) as seeds to the random
* number generator.
*
*  Then you can generate random numbers like so...
*      avx_xorshift128plus(&mykey);
* If your application is threaded, each thread should have its own
* key.
*
*
* The seeds (key1 and key2) should be non-zero. You are responsible for
* checking that they are non-zero.
*/
void avx512_xorshift128plus_init(uint64_t key1, uint64_t key2, avx512_xorshift128plus_key_t *key);

/*
Return a 256-bit random "number"
*/
__m512i avx512_xorshift128plus( avx512_xorshift128plus_key_t *key);

#endif
#endif
