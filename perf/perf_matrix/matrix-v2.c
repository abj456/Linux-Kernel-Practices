#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define N 1024
#define NUM_ROUNDS 5

static int32_t a[N][N];
static int32_t b[N][N];
static int32_t c[N][N];

__attribute__((noinline))
static void load_matrix() {
  FILE *fp = fopen("input.dat", "rb");
  if (!fp) {
    abort();
  }
  if (fread(a, sizeof(a), 1, fp) != 1) {
    abort();
  }
  if (fread(b, sizeof(b), 1, fp) != 1) {
    abort();
  }
  fclose(fp);
}

#if defined(USE_NAIVE)
__attribute__((noinline))
static void mult() {
  size_t i, j, k;
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      for (k = 0; k < N; ++k) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}
#else
static int32_t bT[N][N];

__attribute__((noinline))
static void transpose() {
  size_t i, j;
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      bT[i][j] = b[j][i];
    }
  }
}

__attribute__((noinline))
static void mult() {
  size_t i, j, k;
  transpose();
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      for (k = 0; k < N; ++k) {
        c[i][j] += a[i][k] * bT[j][k];
      }
    }
  }
}
#endif

__attribute__((always_inline))
static inline void escape(void *p) {
  __asm__ volatile ("" : : "r"(p) : "memory");
}

int main() {
  int r;
  load_matrix();
  for (r = 0; r < NUM_ROUNDS; ++r) {
    memset(c, '\0', sizeof(c));
    escape(c);
    mult();
    escape(c);
  }
  return 0;
}
