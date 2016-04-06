# SIMDxorshift


 Xorshift are a family of pseudo-random number generators (RNG) invented by
 Marsaglia.

 https://en.wikipedia.org/wiki/Xorshift


We present   a vectorized version of xorshift128+, a popular random-number generator
part of this family. It is written in C. The implementation uses Intel's SIMD
instructions and is based on Vigna's original (pure C) implementation.

As a random number generator, xorshift128+ is not very strong. It should never be
used alone in applications where the quality of the random numbers matters a great
deal. However, when you just want fast and "good enough" random numbers, it should
do well.

Since speed is the primary benefit of xorshift128+, then it is tempting to accelerate
it further using vector instructions.

## Prerequisite

You should have a recent Intel processor (Haswell or better). If you bought you
PC before everyone on Earth had a smartphone, it is probably too old a PC. Please
upgrade.

Your compiler supports C99, right?

## Code sample

```C
#include "simdxorshift128plus.h"

// create a new key
avx_xorshift128plus_key_t mykey;
avx_xorshift128plus_init(324,4444,&mykey); // values 324, 4444 are arbitrary, must be non-zero

// generate 32 random bytes, do this as many times as you want
__m256i randomstuff =  avx_xorshift128plus(&mykey);
```

## Usage 

```bash
$ make
$ ./fillarray
Generating 5000 32-bit random numbers
Time reported in number of cycles per array element.
We store values to an array of size = 19 kB.

We just generate the random numbers:
populateRandom_xorshift128plus(prec,size):  3.36 cycles per operation
populateRandom_avx_xorshift128plus(prec,size):  1.79 cycles per operation

$ ./shuffle
Shuffling arrays of size 10000
Time reported in number of cycles per array element.
Tests assume that array is in cache as much as possible.
xorshift128plus_shuffle32(&mykey,testvalues,size):  4.67 cycles per operation
avx_xorshift128plus_shuffle32(&myavxkey,testvalues,size):  3.47 cycles per operation
```

(Tests on a Skylake processor.)


## Shallow analysis

SIMD random-number generation is something like twice as fast as plain C random number
generation. However on algorithms such as random shuffling, the benefits of faster random number generation
are lesser because other bottlenecks arise. 

For the most part however, the application of SIMD instructions for random number generation is "free"
if the CPU supports it.


## Reference

Vigna's xorshift128+ implementation http://xorshift.di.unimi.it/xorshift128plus.c
