#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mergesort.h"

/* definitions for the externs declared in mergesort.h */
int* A;
int* B;
int cutoff;

static int failures = 0;

static int cmp_int(const void* a, const void* b) {
  int x = *(const int*)a, y = *(const int*)b;
  return (x > y) - (x < y);
}

/* Sorts a copy of `input` using my_mergesort (cutoff_arg < 0) or
 * parallel_mergesort (cutoff_arg >= 0) and compares against qsort. */
static void run_test(const char* name, const int* input, int n,
                     int cutoff_arg) {
  size_t bytes = (n > 0 ? n : 1) * sizeof(int);
  int* expected = malloc(bytes);
  A = malloc(bytes);
  B = malloc(bytes);
  if (!expected || !A || !B) {
    perror("malloc");
    exit(1);
  }

  if (n > 0) {
    memcpy(A, input, n * sizeof(int));
    memcpy(expected, input, n * sizeof(int));
    qsort(expected, n, sizeof(int), cmp_int);
  }

  if (cutoff_arg < 0) {
    my_mergesort(0, n - 1);
  } else {
    cutoff = cutoff_arg;
    parallel_mergesort(buildArgs(0, n - 1, 0));
  }

  int ok = (n == 0) || memcmp(A, expected, n * sizeof(int)) == 0;
  if (!ok) failures++;
  printf("[%s] %-28s n=%-7d %s%d\n", ok ? "PASS" : "FAIL", name, n,
         cutoff_arg < 0 ? "sequential" : "cutoff=",
         cutoff_arg < 0 ? 0 : cutoff_arg);

  free(expected);
  free(A);
  free(B);
}

/* run one input through sequential + several cutoffs */
static void run_all(const char* name, const int* input, int n) {
  run_test(name, input, n, -1);
  for (int c = 0; c <= 4; c++) run_test(name, input, n, c);
}

int main(void) {
  /* empty and single element */
  run_all("empty", NULL, 0);
  int one[] = {42};
  run_all("single", one, 1);

  /* two elements */
  int two_sorted[] = {1, 2};
  int two_rev[] = {2, 1};
  int two_eq[] = {5, 5};
  run_all("two sorted", two_sorted, 2);
  run_all("two reversed", two_rev, 2);
  run_all("two equal", two_eq, 2);

  /* odd length, even length */
  int odd[] = {5, 3, 9, 1, 7};
  int even[] = {8, 4, 6, 2, 10, 12};
  run_all("odd length", odd, 5);
  run_all("even length", even, 6);

  /* ordering patterns */
  int sorted[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int reversed[] = {8, 7, 6, 5, 4, 3, 2, 1};
  int all_eq[] = {3, 3, 3, 3, 3, 3, 3};
  int dups[] = {4, 1, 4, 2, 1, 2, 4, 1};
  run_all("already sorted", sorted, 8);
  run_all("reverse sorted", reversed, 8);
  run_all("all equal", all_eq, 7);
  run_all("many duplicates", dups, 8);

  /* negatives and extremes */
  int neg[] = {-3, 5, -10, 0, 7, -1};
  int extremes[] = {INT_MAX, 0, INT_MIN, -1, 1, INT_MAX, INT_MIN};
  run_all("negatives", neg, 6);
  run_all("INT_MIN/INT_MAX", extremes, 7);

  /* power-of-two and off-by-one sizes around it */
  for (int n = 3; n <= 33; n++) {
    int* buf = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) buf[i] = rand() % 20 - 10;
    char name[32];
    snprintf(name, sizeof(name), "random small #%d", n);
    run_all(name, buf, n);
    free(buf);
  }

  /* large random input */
  int big_n = 200000;
  int* big = malloc(big_n * sizeof(int));
  for (int i = 0; i < big_n; i++) big[i] = rand();
  run_all("large random", big, big_n);
  free(big);

  printf("\n%s (%d failure%s)\n", failures ? "SOME TESTS FAILED" : "ALL PASSED",
         failures, failures == 1 ? "" : "s");
  return failures ? 1 : 0;
}