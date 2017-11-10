/**
 * @file A simple C source to detect Intel SIMD 256-bit avx instruction support
 *
 * Try to compile with:
 *
 *      gcc -mavx avx.c
 */

#include <immintrin.h>

int main(int argc, char **argv) {
  __m256d a;
  __m256d b;
  __m256d ret = _mm256_add_pd(a, b);

  return 0;
}
