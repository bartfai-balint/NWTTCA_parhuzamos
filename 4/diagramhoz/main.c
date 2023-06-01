#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define MAX_CITIES 5
#define MAX_ANTS 100
#define MAX_ITERATIONS 100
#define ALPHA 1.0
#define BETA 5.0
#define RHO 0.5
#define Q 100
#define NUM_THREADS 4

typedef struct {
    int x;
    int y;
} City;

City cities[MAX_CITIES];
double distances[MAX_CITIES][MAX_CITIES];
double pheromones[MAX_CITIES][MAX_CITIES];
int bestTour[MAX_CITIES];
double bestTourLength;
int numCities;
int numAnts;
int numIterations;
pthread_mutex_t mutex;

void* antColonyOptimization(void* threadId) {
    int tid = *(int*) threadId;
    int tour[MAX_CITIES];

    for (int i = tid; i < numAnts; i += NUM_THREADS) {
        // Initialize the tour with the starting city
        for (int j = 0; j < numCities; j++) {
            tour[j] = j;
        }

        for (int j = 1; j < numCities - 1; j++) {
            int k = j + rand() % (numCities - j);
            int temp = tour[j];
            tour[j] = tour[k];
            tour[k] = temp;
        }

        double tourLength = 0.0;
        for (int j = 0; j < numCities - 1; j++) {
            int city1 = tour[j];
            int city2 = tour[j + 1];
            tourLength += distances[city1][city2];
        }

        pthread_mutex_lock(&mutex);
        if (tourLength < bestTourLength) {
            bestTourLength = tourLength;
            for (int j = 0; j < numCities; j++) {
                bestTour[j] = tour[j];
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void calculateDistances() {
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            double dx = cities[i].x - cities[j].x;
            double dy = cities[i].y - cities[j].y;
            distances[i][j] = sqrt(dx * dx + dy * dy);
        }
    }
}

void initializePheromones() {
    double initialPheromone = 1.0 / (numCities * numCities);

    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            pheromones[i][j] = initialPheromone;
        }
    }
}

void updatePheromones() {
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            pheromones[i][j] *= (1.0 - RHO);
        }
    }

    for (int i = 0; i < numAnts; i++) {
        double contribution = Q / bestTourLength;

        for (int j = 0; j < numCities - 1; j++) {
            int city1 = bestTour[j];
            int city2 = bestTour[j + 1];
            pheromones[city1][city2] += contribution;
            pheromones[city2][city1] += contribution;
        }
    }
}

void printBestTour() {
    printf("Best tour: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d ", bestTour[i]);
    }
    printf("\n");
    printf("Best tour length: %.2f\n", bestTourLength);
}

void printCities() {
    printf("City coordinates:\n");
    for (int i = 0; i < numCities; i++) {
        printf("City %d: (%d, %d)\n", i, cities[i].x, cities[i].y);
    }
}

int main() {


    srand(time(NULL));

    numAnts = MAX_ANTS;
    numIterations = MAX_ITERATIONS;

    cities[0].x = 60; cities[0].y = 200;
    cities[1].x = 180; cities[1].y = 200;
    cities[2].x = 80; cities[2].y = 180;
    cities[3].x = 140; cities[3].y = 180;
    cities[4].x = 20; cities[4].y = 160;

    calculateDistances();
    initializePheromones();

    bestTourLength = INFINITY;

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, antColonyOptimization, &threadIds[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    updatePheromones();
    printBestTour();
    printCities();


    return 0;
}
