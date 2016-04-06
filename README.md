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


## Reference

Vigna's xorshift128+ implementation http://xorshift.di.unimi.it/xorshift128plus.c
