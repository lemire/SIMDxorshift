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
  return _mm256_insertf128_si256(_mm256_castsi128_si256(penultimate2), penultimate1, 1);
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




static __m256i avx_dragonrandombound_epu32(__m256i randomvals, __m256i upperbound) {
	/* four values */
	__m256i evenparts = _mm256_srli_epi64(
			_mm256_mul_epu32(randomvals, upperbound), 32);
	/* four other values */
	__m256i oddparts = _mm256_mul_epu32(_mm256_srli_epi64(randomvals, 32),
			_mm256_srli_epi64(upperbound, 32));
	/* note:shift could be replaced by shuffle */
	/* need to blend the eight values */
	return _mm256_blend_epi32(evenparts, oddparts, 170); //170 == 0b10101010
}

void aesdragontamer_shuffle32(aesdragontamer_state *key,
		uint32_t *storage, uint32_t size) {
	uint32_t i;
	uint32_t randomsource[8];
	__m256i interval = _mm256_setr_epi32(size, size - 1, size - 2, size - 3,
			size - 4, size - 5, size - 6, size - 7);
	__m256i R = avx_dragonrandombound_epu32(aesdragontamer_r(key), interval);
	_mm256_storeu_si256((__m256i *) randomsource, R);
	__m256i vec8 = _mm256_set1_epi32(8);
	for (i = size; i > 1;) {
		for (int j = 0; j < 8; ++j) {
			uint32_t nextpos = randomsource[j];
			int tmp = storage[i - 1]; // likely in cache
			int val = storage[nextpos]; // could be costly
			storage[i - 1] = val;
			storage[nextpos] = tmp; // you might have to read this store later
			i--;
		}
		interval = _mm256_sub_epi32(interval, vec8);
		R = avx_dragonrandombound_epu32(aesdragontamer_r(key), interval);
		_mm256_storeu_si256((__m256i *) randomsource, R);
	}
}

#endif // __AES__
#endif
