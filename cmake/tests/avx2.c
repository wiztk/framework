/**
 * @file A simple C source to detect Intel SIMD 256-bit avx2 instruction support
 *
 * Try to compile with:
 *
 *      gcc -mavx2 avx2.c
 */

#include <immintrin.h>
#include <avx2intrin.h>

int main(int argc, char **argv) {
  __m256i x;
  __m256i y;
  __m256i ret = _mm256_mpsadbw_epu8(x, y, 0);

  return 0;
}
