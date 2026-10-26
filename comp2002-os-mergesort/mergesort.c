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
  // prevent over copy oops
  while (i <= leftend) {
    B[k] = A[i];
    i++;
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

	int mid = left + (right-left)/2;
	my_mergesort(left, mid);
	my_mergesort(mid+1, right);
	//if already ordered, avoid full array copy.
	if (A[mid] > A[mid + 1]){
		merge(left, mid, mid+1, right);
	}
	
  int mid = left + (right - left) / 2;
  my_mergesort(left, mid);
  my_mergesort(mid + 1, right);

  merge(left, mid, mid + 1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
	struct argument *current = (struct argument *)arg;
    struct argument *left_arg;
    struct argument *right_arg;

	pthread_t left_thread;
    pthread_t right_thread;

	int left;
    int right;
    int level;
    int middle;
    int left_result;
    int right_result;
	
	// Check that the argument pointer is valid.
    if (current == NULL) {
        return NULL;
    }

	// base case, no sorting is required for one element.
    if (left >= right) {
        return NULL;
    }

    // If the cutoff level is reached, stop creating
    // threads and use the normal recursive merge sort.
    if (level >= cutoff) {
        my_mergesort(left, right);
        return NULL;
    }

    // Find the midpoint to divide the array into halves.
    middle = left + (right - left) / 2;

    // Create argument structures for both child threads.
    left_arg = buildArgs(left, middle, level + 1);
    right_arg = buildArgs(middle + 1, right, level + 1);

    // Create a thread to sort the left half.
    left_result = pthread_create(
        &left_thread,
        NULL,
        parallel_mergesort,
        left_arg
    );

    // Create a thread to sort the right half.
    right_result = pthread_create(
        &right_thread,
        NULL,
        parallel_mergesort,
        right_arg
    );

    // If thread creation fails, sort that half sequentially.
    if (left_result != 0) {
        my_mergesort(left, middle);
    }

    if (right_result != 0) {
        my_mergesort(middle + 1, right);
    }

    // Wait for the left thread to finish sorting.
    if (left_result == 0) {
        if (pthread_join(left_thread, NULL) != 0) {
            fprintf(stderr, "Failed to join left thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for the right thread to finish sorting.
    if (right_result == 0) {
        if (pthread_join(right_thread, NULL) != 0) {
            fprintf(stderr, "Failed to join right thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Both child threads have finished using their arguments,
    // so their allocated memory can now be safely freed.
    free(left_arg);
    free(right_arg);

    // Merge only after both halves have finished sorting.
    // This ensures correct thread synchronisation.
    merge(left, middle, middle + 1, right);

    return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument* buildArgs(int left, int right, int level) {
  struct argument* args = malloc(sizeof(struct argument));
  if (args == NULL) {
    perror("malloc");
    exit(1);
  }
  args->left = left;
  args->right = right;
  args->level = level;
  return args;
  return NULL;
}
