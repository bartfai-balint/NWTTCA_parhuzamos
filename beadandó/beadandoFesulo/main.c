#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_CITIES 100
#define MAX_ITERATIONS 100
#define ALPHA 1.0
#define BETA 5.0
#define RHO 0.5
#define Q 100.0

typedef struct {
    double x;
    double y;
} City;

typedef struct {
    int cities[MAX_CITIES];
    double length;
} Ant;

City cities[MAX_CITIES];
double pheromones[MAX_CITIES][MAX_CITIES];

double distance(City city1, City city2) {
    double dx = city1.x - city2.x;
    double dy = city1.y - city2.y;
    return sqrt(dx * dx + dy * dy);
}

void initialize_pheromones(int num_cities) {
    int i, j;
    for (i = 0; i < num_cities; i++) {
        for (j = 0; j < num_cities; j++) {
            pheromones[i][j] = 0.1;
        }
    }
}

void update_pheromones(int num_cities, Ant *ants, double evaporation) {
    int i, j;
    for (i = 0; i < num_cities; i++) {
        for (j = 0; j < num_cities; j++) {
            pheromones[i][j] *= (1.0 - evaporation);
        }
    }

    for (i = 0; i < num_cities; i++) {
        int city1 = ants[i].cities[num_cities - 1];
        int city2 = ants[i].cities[0];
        pheromones[city1][city2] += (Q / ants[i].length);

        for (j = 0; j < num_cities - 1; j++) {
            city1 = ants[i].cities[j];
            city2 = ants[i].cities[j + 1];
            pheromones[city1][city2] += (Q / ants[i].length);
        }
    }
}

void construct_solution(int num_cities, int num_ants, double alpha, double beta, Ant *ants) {
    int i, j, k;
    double probabilities[MAX_CITIES];

    for (k = 0; k < num_ants; k++) {
        int start_city = rand() % num_cities;
        int visited[num_cities];
        for (i = 0; i < num_cities; i++) {
            visited[i] = 0;
        }

        ants[k].cities[0] = start_city;
        visited[start_city] = 1;

        for (i = 0; i < num_cities - 1; i++) {
            double sum = 0.0;
            for (j = 0; j < num_cities; j++) {
                if (visited[j] == 0) {
                    double pheromone = pow(pheromones[start_city][j], alpha);
                    double dist = pow(1.0 / distance(cities[start_city], cities[j]), beta);
                    probabilities[j] = pheromone * dist;
                    sum += probabilities[j];
                } else {
                    probabilities[j] = 0.0;
                }
            }

            double random = (double)rand() / RAND_MAX;
            double p = 0.0;
            for (j = 0; j < num_cities; j++) {
                probabilities[j] /= sum;
                p += probabilities[j];
                if (random <= p) {
                    ants[k].cities[i + 1] = j;
                    visited[j] = 1;
                    break;
                }
            }
            start_city = ants[k].cities[i + 1];
        }

        ants[k].length = 0.0;
        for (i = 0; i < num_cities - 1; i++) {
            ants[k].length += distance(cities[ants[k].cities[i]], cities[ants[k].cities[i + 1]]);
        }
        ants[k].length += distance(cities[ants[k].cities[num_cities - 1]], cities[ants[k].cities[0]]);
    }
}

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (cities[L[i]].x <= cities[R[j]].x) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main() {
    int num_cities, num_ants;
    printf("Enter the number of cities: ");
    scanf("%d", &num_cities);

    printf("Enter the number of ants: ");
    scanf("%d", &num_ants);

    srand(time(NULL));

    int i;
    for (i = 0; i < num_cities; i++) {
        cities[i].x = (double)(rand() % 1000);
        cities[i].y = (double)(rand() % 1000);
    }

    Ant ants[num_ants];
    Ant best_ant;
    best_ant.length = 1e9;

    initialize_pheromones(num_cities);

    int iteration;
    for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        construct_solution(num_cities, num_ants, ALPHA, BETA, ants);

        for (i = 0; i < num_ants; i++) {
            if (ants[i].length < best_ant.length) {
                best_ant = ants[i];
            }
        }

        update_pheromones(num_cities, ants, RHO);
    }

    printf("Best path found:\n");
    for (i = 0; i < num_cities; i++) {
        printf("%d ", best_ant.cities[i]);
    }
    printf("\n");

    printf("Length of the best path: %.2f\n", best_ant.length);

    printf("Coordinates of all cities:\n");
    for (i = 0; i < num_cities; i++) {
        printf("City %d: (%.2f, %.2f)\n", i, cities[i].x, cities[i].y);
    }

    return 0;
}

