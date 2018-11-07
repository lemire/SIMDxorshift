/***
* Code from Percy "Dragontamer" Tiglao
***/

#ifndef AESDRAGONTAMER_H
#define AESDRAGONTAMER_H
#if defined( __AES__) && defined(__AVX__) 

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>

typedef struct {
  __m128i state;
  __m128i increment;
} aesdragontamer_state;


static inline __m256i aesdragontamer_r(aesdragontamer_state *state) {
  state->state = _mm_add_epi64(state->state, state->increment);
  __m128i penultimate = _mm_aesenc_si128(state->state, state->increment); 
  __m128i penultimate1 = _mm_aesenc_si128(penultimate, state->increment); 
  __m128i penultimate2 = _mm_aesdec_si128(penultimate, state->increment); 
  return _mm256_set_m128i(penultimate1,penultimate2);
}


static inline void aesdragontamer_seed_r(uint64_t seed1, uint64_t seed2, aesdragontamer_state *state) {
  state->increment = _mm_set_epi8(0x2f, 0x2b, 0x29, 0x25, 0x1f, 0x1d, 0x17, 0x13, 
		0x11, 0x0D, 0x0B, 0x07, 0x05, 0x03, 0x02, 0x01);
  state->state = _mm_set_epi64x(seed1, seed2);
}



static aesdragontamer_state g_aesdragontamer_state;

static inline void aesdragontamer_seed(uint64_t seed1, uint64_t seed2) {
  aesdragontamer_seed_r(seed1, seed2, &g_aesdragontamer_state);
}

static inline __m256i aesdragontamer() { return aesdragontamer_r(&g_aesdragontamer_state); }

#endif // __AES__
#endif
