Operating Systems Assignment2 README

## Project Number/Title 

* Authors: Brandon Ho, Jeremy Nguyen, Marko Bogsav
* Group name: brandon

## Overview

This project implements a parallel merge sort algorithm using pthreads. 
The program works by dividing an array into smaller sections that can be sorted concurrently and then merges the sorted sections into a final array.
The buildArgs has the option to cutoff level controls for thread creation, allowing the performance of sequential and parallel sorting to be compared. 


## Manifest

A listing of source files and other non-generated files, and a brief
(one-line) explanation of the purpose of each file.

**The project contains the following source files:**
mergesort.c -> Implements sequential and parallel merge sort algorithms. Includes merge function and thread management.
mergesort.h -> Contains declarations, shared variables and structures required by the merge sort implementation.
test-mergesort.c -> Testing file used to test the final, completed implementations.
mergesting.c -> Generated testing file, used when progressively implementing the mergesort functions.
Makefile -> Contains the the instructions to compile and build the project.
README -> Documentation covering the project.
run_tests.sh -> Automatic testing script which tests 133 different cases.

## Building the project

This project is designed to compile and run in a Linux environment. The testing and creation of this project was performed through Ubuntu inside the Windows Subsystem for Linux (WSL).

**The prerequistes:**
- GCC Compiler
- GNU Make
- POSIX Threads (Pthreads)
- Linux or WSL environment

**Compilation Steps:**
1) Open Terminal and navigate to the directory containing make file (/operating-systems-1/comp2002-os-mergesort)
2) Compile using the command "make"
3) If the run_tests.sh script is to be ran. Ensure "chmod +x run_tests.sh" and "make clean" is executed prior

**Post-Compilation:**
1) To run the program: ./test-mergesort (SORT_AMOUNT, THREAD_MAXIMUM, SEED) OR ./run_tests.sh for automatic testing.
2) To clean the testing files: "make clean"

## Features and usage

Implementation of merge sort using both sequential recursion and parallel execution.

**Sequential Merge Sort:**

The my_mergesort() function recursively divides an array into smaller sections until all sections contain one element. Then the sections are merged together in sorted order.

The merge() function uses a temporary array to combine two sorted sections, copying the resulting array values back into the original array. 

**Parallel Merge Sort:**

The parallel_mergesort() functions uses POSIX threads to sort seperate sections of the array concurrently.

**The main functions include:**
- Recursive division of the array into smaller sections
- Concurrent sorting using pthread_create()
- Synchronisation using pthread_join() to ensure both sections are sorted before merging.
- A custom cutoff level to limit the thread creation.
- Sequential sorting when the cutoff level is reached.
- Fallback to sequential sorting if thread creation fails.
- Dynamic allocation of thread argument structures using malloc(), followed by memory cleanup using free().

**Cut-off level:**
The cutoff determines recursion depth at which the program stops creating additional threads and switches to sequential merge sort.

**Example 1:**
cutoff = 0: Array is sorted sequentially
**Example 2:**
cutoff = 1: The array is divided into two sections that can be sorted by separate threads.
**Example 3:**
cutoff = 2: The array can be divided into four sorting sections at the second level.

Increasing the cutoff allows more parallel execution but also introduces more overhead from creating and managing threads.

## Testing

Testing focuses on verifying sorting correctness, checking edge cases, and making sure the parallel implementation behaves correctly across different cutoff levels.

1. **Sorting Correctness**

The program should be tested using arrays with different arrangements of values, including:
- Randomly ordered integers.
- Already sorted arrays.
- Reverse-sorted arrays.
- Arrays containing duplicate values.
- Arrays containing negative integers.
- The expected result is an array sorted in ascending order.

**Example:**

Input:
[5, 2, 8, 1, 3]
Expected output:
[1, 2, 3, 5, 8]

2. **Edge Cases**

The implementation should also be tested with:
- Empty arrays.
- Arrays containing a single element.
- Arrays containing two elements.
-  Arrays containing identical elements.

These tests check whether the base case is handled correctly and whether the algorithm avoids invalid memory access.

3. **Sequential and Parallel Testing**

The program should be tested with multiple cutoff levels.
Starting with cutoff = 0 allows the sequential implementation to be verified independently of thread creation.
Higher cutoff values, including 1, 2, and 3, can then be tested to confirm that multiple threads correctly sort their assigned sections before merging.
The output should remain identical regardless of the cutoff value.

4. **Performance Testing**

Performance can be evaluated by comparing execution times between the sequential implementation and the parallel implementation using larger input arrays.
Different cutoff levels can be compared to assess whether the benefit of parallel execution outweighs the overhead of creating and synchronising threads.

Testing results: All 133 tests passed. Testing comprised of 78 boundary tests, 40 random-seed tests and 15 stress tests.
**One important limitation**: the test program checks whether the output is ordered, but that alone does not prove that every original element was preserved. For comprehensive correctness, an independent comparison against a trusted sorted copy would be stronger.


## Known Bugs

As of writing this there have not been any known bugs at the moment.

During development the program did initially fail sorting tests due to issues in the implementation. These included uninitialised variables within the parallel sorting function and also duplicate recursive calls. I suspect this was due to merging conflicts without properly clearing. 

These bugs were quickly resolved and squashed during a team meeting where we conducted a code review.

## Reflection and Self Assessment

One of the main challenges during development was understanding how merge sort operates recursively and how the algorithm can be adapted for parallel execution.

Initially, understanding how the left and right sections were merged was a struggle, (mostly it was visualising), particularly when managing array indices. The merge() function required careful handling of the left, right, and temporary array positions to ensure that elements were copied correctly without being skipped or overwritten.

Another challenge was implementing multithreading using pthreads. We needed to understand how to pass arguments to threads, how recursion works when separate threads are created and the importance of syncronisation to avoid any race conditions.

During testing, the program initially reported that sorting had failed. Reviewing the code revealed that some variables in parallel_mergesort() had been declared without being initialised from the argument structure. This led to errors as the values would be completely random per run. This demonstrated the importance of initialising variables before they are used.

We also identified duplicated recursive calls in my_mergesort(), which resulted in unnecessary sorting operations. Removing the duplicated calls simplified the implementation and reduced unnecessary system work.

An important concept we developed a better understanding of was the relationship between concurrency and synchronisation. Although separate sections of an array can be sorted concurrently, the merging process must wait until both sections have been fully sorted. The use of pthread_join() ensures this condition is met and avoids the race conditions which lead to inaccurate and random results.

Overall, the assignment provided practical experience with recursive algorithms, memory management, POSIX threads, and debugging C programs. It also reinforced the importance of testing the sequential implementation before introducing parallel execution.

## Sources Used

If you used any sources outside of the textbook, you should list them here. 
If you looked something up on stackoverflow.com or you use help from AI, and 
fail to cite it in this section, it will be considered plagiarism and dealt 
with accordingly. So be safe CITE!

The C-Standard Library was used: Reference material for memory management and operations such as malloc(), free(), and memcpy(). 
Adelaide University course materials were also used.

https://www.sjsu.edu/people/robert.chun/courses/cs159/s3/T.pdf - Parallel MergeSort
https://medium.com/@angadbir0987/understanding-parallel-merge-sort-efficiency-and-performance-148107c32ad5 - Comparing Parallel and Sequential
https://en.cppreference.com/c/memory - C-standard library


## AI Declaration
AI was used to understand the concepts of race conditions, merge sort, recursive algorithms and just explaining differences from C++ to C.

AI was also used to develop the run_tests.sh file to automatically test on a bunch of cases and have it save to a .csv file.
