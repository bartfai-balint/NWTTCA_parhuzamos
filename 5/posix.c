#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 10

double arr[N];

typedef struct {
    int start;
    int end;
    double product;
} thread_arg;

void* array_product(void* arg) {
    thread_arg* t_arg = (thread_arg*) arg;
    for (int i = t_arg->start; i <= t_arg->end; i++) {
        t_arg->product *= arr[i];
    }
    return NULL;
}

int main(int argc, char* argv[]) {

    clock_t begin = clock();
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        arr[i] = (double)rand() / RAND_MAX * 10.0;
    }

    int num_threads = 10;

    pthread_t threads[num_threads];
    thread_arg args[num_threads];
    double products[num_threads];
    int chunk_size = N / num_threads;
    for (int i = 0; i < num_threads; i++) {
        args[i].start = i * chunk_size;
        args[i].end = (i == num_threads-1) ? N-1 : args[i].start + chunk_size-1;
        args[i].product = 1.0;
        products[i] = 1.0;
        pthread_create(&threads[i], NULL, array_product, &args[i]);
    }

    double product = 1.0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        product *= args[i].product;
    }

    printf("Product with posix threads: %.2f\n", product);

    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time with posix threads: %f", timeSpent);

    return 0;
}