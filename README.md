# ADV-OPT-SYS
# Parallel Processing with Pthreads

## Overview
This project implements parallelized operations using **POSIX threads (pthreads)** in C. The program provides three core functions:

1. **Parallel Array Summation (`parallel_sum_array`)** – Computes the element-wise sum of two arrays using multiple threads.
2. **Parallel Total Summation (`parallel_sum`)** – Computes the total sum of an array in parallel.
3. **Parallel Sorting (`parallel_sort`)** – Sorts an array using multiple threads and merges the sorted segments.

## Features
- Uses **pthreads** for efficient parallel computation.
- Splits workload into **multiple threads** to optimize performance.
- Implements **merge sorting** for sorting efficiency.

## Compilation Instructions
To compile the program, use **GCC** with the pthread library:

```sh
gcc -pthread parallel_program.c -o parallel_program
