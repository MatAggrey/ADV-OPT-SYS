#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Structure for parallel sum of two arrays
typedef struct {
    long *a, *b, *c;
    ssize_t start, end;
} SumArrayArgs;

void *sum_array_worker(void *arg) {
    SumArrayArgs *args = (SumArrayArgs *)arg;
    for (ssize_t i = args->start; i < args->end; i++) {
        args->c[i] = args->a[i] + args->b[i];
    }
    return NULL;
}

void parallel_sum_array(int num_threads, long *a, long *b, long *c, ssize_t size) {
    pthread_t threads[num_threads];
    SumArrayArgs args[num_threads];
    ssize_t chunk_size = size / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].c = c;
        args[i].start = i * chunk_size;
        args[i].end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, sum_array_worker, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Structure for parallel sum of array elements
typedef struct {
    long *ary;
    ssize_t start, end;
    long partial_sum;
} SumArgs;

void *sum_worker(void *arg) {
    SumArgs *args = (SumArgs *)arg;
    args->partial_sum = 0;
    for (ssize_t i = args->start; i < args->end; i++) {
        args->partial_sum += args->ary[i];
    }
    return NULL;
}

long parallel_sum(int num_threads, long *ary, ssize_t array_size) {
    pthread_t threads[num_threads];
    SumArgs args[num_threads];
    ssize_t chunk_size = array_size / num_threads;
    long total_sum = 0;

    for (int i = 0; i < num_threads; i++) {
        args[i].ary = ary;
        args[i].start = i * chunk_size;
        args[i].end = (i == num_threads - 1) ? array_size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, sum_worker, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += args[i].partial_sum;
    }

    return total_sum;
}

// Structure for sorting
typedef struct {
    long *ary;
    ssize_t start, end;
} SortArgs;

int compare(const void *a, const void *b) {
    return (*(long *)a - *(long *)b);
}

void *sort_worker(void *arg) {
    SortArgs *args = (SortArgs *)arg;
    qsort(args->ary + args->start, args->end - args->start, sizeof(long), compare);
    return NULL;
}

void merge(long *ary, ssize_t left, ssize_t mid, ssize_t right) {
    ssize_t i = left, j = mid, k = 0;
    ssize_t size = right - left;
    long *temp = (long *)malloc(size * sizeof(long));

    while (i < mid && j < right) {
        temp[k++] = (ary[i] < ary[j]) ? ary[i++] : ary[j++];
    }
    while (i < mid) temp[k++] = ary[i++];
    while (j < right) temp[k++] = ary[j++];

    for (i = 0; i < size; i++) {
        ary[left + i] = temp[i];
    }
    free(temp);
}

void parallel_sort(int num_threads, long *ary, ssize_t array_size) {
    pthread_t threads[num_threads];
    SortArgs args[num_threads];
    ssize_t chunk_size = array_size / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].ary = ary;
        args[i].start = i * chunk_size;
        args[i].end = (i == num_threads - 1) ? array_size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, sort_worker, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int step = chunk_size; step < array_size; step *= 2) {
        for (int i = 0; i < array_size; i += 2 * step) {
            ssize_t mid = i + step;
            ssize_t right = (i + 2 * step > array_size) ? array_size : i + 2 * step;
            if (mid < right) merge(ary, i, mid, right);
        }
    }
}
