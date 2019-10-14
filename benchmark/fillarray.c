#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simdxorshift128plus.h"
#include "xorshift128plus.h"
#include "simdaesdragontamer.h"

void populateRandom_xorshift128plus(uint32_t *answer, uint32_t size) {
  xorshift128plus_key_t mykey = {.part1 = 324, .part2 = 4444};
  xorshift128plus_init(324, 4444, &mykey);
  uint32_t i = size;
  while (i > 2) {
    *(uint64_t *)(answer + size - i) = xorshift128plus(&mykey);
    i -= 2;
  }
  if (i != 0)
    answer[size - i] = (uint32_t)xorshift128plus(&mykey);
}

void populateRandom_avx_aesdragontamer(uint32_t *answer, uint32_t size) {
  uint32_t i = 0;
  aesdragontamer_state mykey;
  aesdragontamer_seed_r(324, 4444, &mykey);

  const uint32_t block = sizeof(__m256i) / sizeof(uint32_t); // 8
  while (i + block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), aesdragontamer_r(&mykey));
    i += block;
  }
  if (i != size) {
    uint32_t buffer[sizeof(__m256i) / sizeof(uint32_t)];
    _mm256_storeu_si256((__m256i *)buffer, aesdragontamer_r(&mykey));
    memcpy(answer + i, buffer, sizeof(uint32_t) * (size - i));
  }
}

void populateRandom_avx_xorshift128plus(uint32_t *answer, uint32_t size) {
  uint32_t i = 0;
  avx_xorshift128plus_key_t mykey;
  avx_xorshift128plus_init(324, 4444, &mykey);

  const uint32_t block = sizeof(__m256i) / sizeof(uint32_t); // 8
  while (i + block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), avx_xorshift128plus(&mykey));
    i += block;
  }
  if (i != size) {
    uint32_t buffer[sizeof(__m256i) / sizeof(uint32_t)];
    _mm256_storeu_si256((__m256i *)buffer, avx_xorshift128plus(&mykey));
    memcpy(answer + i, buffer, sizeof(uint32_t) * (size - i));
  }
}

void populateRandom_avx_xorshift128plus_two(uint32_t *answer, uint32_t size) {
  uint32_t i = 0;
  avx_xorshift128plus_key_t mykey1;
  avx_xorshift128plus_init(324, 4444, &mykey1);
  avx_xorshift128plus_key_t mykey2;
  avx_xorshift128plus_init(3244444, 444774, &mykey2);

  const uint32_t block = sizeof(__m256i) / sizeof(uint32_t); // 8
  while (i + 2 * block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), avx_xorshift128plus(&mykey1));
    _mm256_storeu_si256((__m256i *)(answer + i + block),
                        avx_xorshift128plus(&mykey2));
    i += 2 * block;
  }
  while (i + block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), avx_xorshift128plus(&mykey1));
    i += block;
  }
  if (i != size) {
    uint32_t buffer[sizeof(__m256i) / sizeof(uint32_t)];
    _mm256_storeu_si256((__m256i *)buffer, avx_xorshift128plus(&mykey1));
    memcpy(answer + i, buffer, sizeof(uint32_t) * (size - i));
  }
}
#if defined(__AVX512F__)
void populateRandom_avx512_xorshift128plus_two(uint32_t *answer,
                                               uint32_t size) {
  uint32_t i = 0;
  avx512_xorshift128plus_key_t mykey1;
  avx512_xorshift128plus_init(324, 4444, &mykey1);
  avx512_xorshift128plus_key_t mykey2;
  avx512_xorshift128plus_init(3244444, 444774, &mykey2);

  const uint32_t block = sizeof(__m512i) / sizeof(uint32_t); // 16
  while (i + 2 * block <= size) {
    _mm512_storeu_si512((__m512i *)(answer + i),
                        avx512_xorshift128plus(&mykey1));
    _mm512_storeu_si512((__m512i *)(answer + i + block),
                        avx512_xorshift128plus(&mykey2));
    i += 2 * block;
  }
  while (i + block <= size) {
    _mm512_storeu_si512((__m512i *)(answer + i),
                        avx512_xorshift128plus(&mykey1));
    i += block;
  }
  if (i != size) {
    uint32_t buffer[sizeof(__m512i) / sizeof(uint32_t)];
    _mm512_storeu_si512((__m512i *)buffer, avx512_xorshift128plus(&mykey1));
    memcpy(answer + i, buffer, sizeof(uint32_t) * (size - i));
  }
}
void populateRandom_avx512_xorshift128plus_four(uint32_t *answer,
                                                uint32_t size) {
  uint32_t i = 0;
  avx512_xorshift128plus_key_t mykey1;
  avx512_xorshift128plus_init(324, 4444, &mykey1);
  avx512_xorshift128plus_key_t mykey2;
  avx512_xorshift128plus_init(3244444, 444774, &mykey2);
  avx512_xorshift128plus_key_t mykey3;
  avx512_xorshift128plus_init(53244444, 5444774, &mykey3);
  avx512_xorshift128plus_key_t mykey4;
  avx512_xorshift128plus_init(13244444, 4444774, &mykey4);

  const uint32_t block = sizeof(__m512i) / sizeof(uint32_t); // 16
  while (i + 4 * block <= size) {
    _mm512_storeu_si512((__m512i *)(answer + i),
                        avx512_xorshift128plus(&mykey1));
    _mm512_storeu_si512((__m512i *)(answer + i + block),
                        avx512_xorshift128plus(&mykey2));
    _mm512_storeu_si512((__m512i *)(answer + i + 2 * block),
                        avx512_xorshift128plus(&mykey3));
    _mm512_storeu_si512((__m512i *)(answer + i + 3 * block),
                        avx512_xorshift128plus(&mykey4));
    i += 4 * block;
  }
  while (i + 2 * block <= size) {
    _mm512_storeu_si512((__m512i *)(answer + i),
                        avx512_xorshift128plus(&mykey1));
    _mm512_storeu_si512((__m512i *)(answer + i + block),
                        avx512_xorshift128plus(&mykey2));
    i += 2 * block;
  }
  while (i + block <= size) {
    _mm512_storeu_si512((__m512i *)(answer + i),
                        avx512_xorshift128plus(&mykey1));
    i += block;
  }
  if (i != size) {
    uint32_t buffer[sizeof(__m512i) / sizeof(uint32_t)];
    _mm512_storeu_si512((__m512i *)buffer, avx512_xorshift128plus(&mykey1));
    memcpy(answer + i, buffer, sizeof(uint32_t) * (size - i));
  }
}

#endif

void populateRandom_avx_xorshift128plus_four(uint32_t *answer, uint32_t size) {
  uint32_t i = 0;
  avx_xorshift128plus_key_t mykey1;
  avx_xorshift128plus_init(324, 4444, &mykey1);
  avx_xorshift128plus_key_t mykey2;
  avx_xorshift128plus_init(3244444, 444774, &mykey2);
  avx_xorshift128plus_key_t mykey3;
  avx_xorshift128plus_init(324444444, 4244774, &mykey3);
  avx_xorshift128plus_key_t mykey4;
  avx_xorshift128plus_init(324422444, 4447274, &mykey4);

  const uint32_t block = sizeof(__m256i) / sizeof(uint32_t); // 8
  while (i + 4 * block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), avx_xorshift128plus(&mykey1));
    _mm256_storeu_si256((__m256i *)(answer + i + block),
                        avx_xorshift128plus(&mykey2));
    _mm256_storeu_si256((__m256i *)(answer + i + 2 * block),
                        avx_xorshift128plus(&mykey3));
    _mm256_storeu_si256((__m256i *)(answer + i + 3 * block),
                        avx_xorshift128plus(&mykey4));
    i += 4 * block;
  }
  while (i + 2 * block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), avx_xorshift128plus(&mykey1));
    _mm256_storeu_si256((__m256i *)(answer + i + block),
                        avx_xorshift128plus(&mykey2));
    i += 2 * block;
  }
  while (i + block <= size) {
    _mm256_storeu_si256((__m256i *)(answer + i), avx_xorshift128plus(&mykey1));
    i += block;
  }
  if (i != size) {
    uint32_t buffer[sizeof(__m256i) / sizeof(uint32_t)];
    _mm256_storeu_si256((__m256i *)buffer, avx_xorshift128plus(&mykey1));
    memcpy(answer + i, buffer, sizeof(uint32_t) * (size - i));
  }
}

#define RDTSC_START(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("cpuid\n\t"                                                 \
                   "rdtsc\n\t"                                                 \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

#define RDTSC_FINAL(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("rdtscp\n\t"                                                \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   "cpuid\n\t"                                                 \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

/*
 * Prints the best number of operations per cycle where
 * test is the function call, answer is the expected answer generated by
 * test, repeat is the number of times we should repeat and size is the
 * number of operations represented by test.
 */
#define BEST_TIME(test, pre, repeat, size)                                     \
  do {                                                                         \
    printf("%s: ", #test);                                                     \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      test;                                                                    \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = size;                                                         \
    float cycle_per_op = (min_diff) / (double)S;                               \
    printf(" %.2f cycles per operation", cycle_per_op);                        \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)

void demo(int size) {
  printf("Generating %d 32-bit random numbers \n", size);
  printf("Time reported in number of cycles per array element.\n");
  printf("We store values to an array of size = %lu kB.\n",
         size * sizeof(uint32_t) / (1024));
  int repeat = 50;
  uint32_t *prec = malloc(size * sizeof(uint32_t));
  uint32_t *tmp = malloc(size * sizeof(uint32_t));
  for(size_t i = 0; i < (size_t)size; i++) tmp[i] = i;
  printf("\nWe just generate the random numbers: \n");
  BEST_TIME(memcpy(prec,tmp,size * sizeof(uint32_t)), , repeat, size);
  BEST_TIME(populateRandom_xorshift128plus(prec, size), , repeat, size);
  if(prec[1] == 0) printf("%u\n",prec[1]);
  BEST_TIME(populateRandom_avx_aesdragontamer(prec, size), , repeat, size);
  if(prec[1] == 0) printf("%u\n",prec[1]);
  BEST_TIME(populateRandom_avx_xorshift128plus(prec, size), , repeat, size);
  if(prec[1] == 0) printf("%u\n",prec[1]);
  BEST_TIME(populateRandom_avx_xorshift128plus_two(prec, size), , repeat, size);
#if defined(__AVX512F__)
  BEST_TIME(populateRandom_avx512_xorshift128plus_two(prec, size), , repeat,
            size);
  if(prec[1] == 0) printf("%u\n",prec[1]);
 #else
  printf("AVX-512 not available.\n");
#endif
  free(prec);
  free(tmp);
  printf("\n");
}

int main() {
  demo(50000);
  return 0;
}
