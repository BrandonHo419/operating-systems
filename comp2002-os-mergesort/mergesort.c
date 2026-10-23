/**
 * This file implements parallel mergesort.
 */

#include "mergesort.h"

#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h> /* for memcpy */

/* this function will be called by mergesort() and also by parallel_mergesort().
 */
void merge(int leftstart, int leftend, int rightstart, int rightend) {
  int i = leftstart;
  int j = rightstart;
  int k = leftstart;

  while (i <= leftend && j <= rightend) {
    if (A[i] <= A[j]) {
      B[k] = A[i];
      i++;
    } else {
      B[k] = A[j];
      j++;
    }
    k++;
  }

  // copy right side elements
  while (j <= rightend) {
    B[k] = A[j];
    j++;
    k++;
  }

  // copy merged back into A (use memcpy to avoid deleting element)

  memcpy(A + leftstart, B + leftstart,
         (rightend - leftstart + 1) * sizeof(int));
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right) {
  if (left >= right) {
    return;  // base case
  }

  int mid = left + (right - left) / 2;
  my_mergesort(left, mid);
  my_mergesort(mid + 1, right);

  merge(left, mid, mid + 1, right);
}

/* this function will be called by the testing program. */
void* parallel_mergesort(void* arg) { return NULL; }

/* we build the argument for the parallel_mergesort function. */
struct argument* buildArgs(int left, int right, int level) { return NULL; }
