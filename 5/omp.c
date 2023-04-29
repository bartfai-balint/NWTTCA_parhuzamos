#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define NUM_ELEMENTS 1000000
#define NUM_THREADS 4

int main() {
    clock_t begin = clock();
    double array[NUM_ELEMENTS];
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        array[i] = (double)rand() / RAND_MAX;
    }

    double product = 1.0;
    #pragma omp parallel for num_threads(NUM_THREADS) reduction(*:product)
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        product *= array[i];
    }

    printf("Product with omp: %f\n", product);

    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time with posix threads: %f", timeSpent);

    return 0;
}