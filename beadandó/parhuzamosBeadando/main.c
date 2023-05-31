#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX_CITIES 100
#define MAX_ANTS 100
#define ALPHA 1.0
#define BETA 3.0
#define RHO 0.5
#define QVAL 100
#define MAX_TOUR (MAX_CITIES * MAX_CITIES)
#define MAX_DIST 10000

int numCities;
int numAnts;
int cities[MAX_CITIES][2];
double pheromones[MAX_CITIES][MAX_CITIES];
double dist[MAX_CITIES][MAX_CITIES];
int bestTour[MAX_CITIES];
double bestTourLength;

double antProduct[MAX_ANTS][MAX_CITIES];
int antTour[MAX_ANTS][MAX_CITIES];
double antLength[MAX_ANTS];

void initialize();
void constructAntSolutions();
void updatePheromones();
void displayResult();
void displayCities();

double randDouble();
int randInt(int n);
double distance(int cityX, int cityY);
int nearestNeighbor(int city);

int main() {
    int maxIterations = 100;
    int iteration;

    printf("Number of cities: ");
    scanf("%d", &numCities);

    printf("Number of ants: ");
    scanf("%d", &numAnts);

    printf("Number of iterations: ");
    scanf("%d", &maxIterations);

    initialize();

    for (iteration = 0; iteration < maxIterations; iteration++) {
        constructAntSolutions();
        updatePheromones();

         // Frissíti a legjobb útvonalat és annak hosszát
        for (int ant = 0; ant < numAnts; ant++) {
            if (antLength[ant] < bestTourLength) {
                bestTourLength = antLength[ant];
                for (int i = 0; i < numCities; i++) {
                    bestTour[i] = antTour[ant][i];
                }
            }
        }
    }

    displayResult();
    displayCities();

    return 0;
}

void initialize() {
    int from, to;

    for (from = 0; from < numCities; from++) {
        for (to = 0; to < numCities; to++) {
            pheromones[from][to] = 0.01;
            dist[from][to] = MAX_DIST;
        }
    }

    for (from = 0; from < numCities; from++) {
        int x = randInt(100);
        int y = randInt(100);
        cities[from][0] = x;
        cities[from][1] = y;

        for (to = 0; to < numCities; to++) {
            if (to != from) {
                double d = distance(from, to);
                dist[from][to] = d;
                dist[to][from] = d;
            }
        }
    }

    bestTourLength = MAX_DIST * (numCities - 1);

    for (int ant = 0; ant < numAnts; ant++) {
        antLength[ant] = bestTourLength;
    }

}

void constructAntSolutions() {
    int ant;

    for (ant = 0; ant < numAnts; ant++) {
        int i, j;
        int startCity = randInt(numCities);

        for (i = 0; i < numCities; i++) {
            antProduct[ant][i] = 0.0;
            antTour[ant][i] = -1;
        }

        antLength[ant] = 0.0;
        antTour[ant][0] = startCity;
        antTour[ant][numCities - 1] = startCity;

        for (i = 1; i < numCities; i++) {
            int nextCity;
            double denom = 0.0;

            for (j = 0; j < numCities; j++) {
                if (antTour[ant][j] == -1) {
                    double prod = pow(pheromones[startCity][j], ALPHA) * pow((1.0 / dist[startCity][j]), BETA);
                    antProduct[ant][j] = prod;
                    denom += prod;
                }
            }

            double p = randDouble();
            double sum = 0.0;

            for (j = 0; j < numCities; j++) {
                if (antTour[ant][j] == -1) {
                    sum += antProduct[ant][j] / denom;

                    if (sum >= p) {
                        nextCity = j;
                        break;
                    }
                }
            }

            antTour[ant][i] = nextCity;
            antLength[ant] += dist[startCity][nextCity];
            startCity = nextCity;
        }
    }
}

void updatePheromones() {
    int from, to, i, ant;

    for (from = 0; from < numCities; from++) {
        for (to = 0; to < numCities; to++) {
            if (from != to) {
                pheromones[from][to] *= (1.0 - RHO);
                pheromones[from][to] = fmax(pheromones[from][to], 0.01);
            }
        }
    }

    for (ant = 0; ant < numAnts; ant++) {
        double antContribution = QVAL / antLength[ant];

        for (i = 0; i < numCities - 1; i++) {
            from = antTour[ant][i];
            to = antTour[ant][i + 1];
            pheromones[from][to] += antContribution;
            pheromones[to][from] = pheromones[from][to];
        }
    }
}

void displayResult() {
    int i;

    printf("\nBest tour: ");

    for (i = 0; i < numCities; i++) {
        printf("%d ", bestTour[i]);
    }

    printf("\nBest tour length: %.2f\n", bestTourLength);
}

void displayCities() {
    printf("City coordinates:\n");
    for (int i = 0; i < numCities; i++) {
        printf("City %d: (%d, %d)\n", i, cities[i][0], cities[i][1]);
    }
}


double randDouble() {
    return (double)rand() / (double)RAND_MAX;
}

int randInt(int n) {
    return rand() % n;
}

double distance(int cityX, int cityY) {
    int x1 = cities[cityX][0];
    int y1 = cities[cityX][1];
    int x2 = cities[cityY][0];
    int y2 = cities[cityY][1];
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

int nearestNeighbor(int city) {
    int nearestCity = -1;
    double minDist = MAX_DIST;

    int i;
    for (i = 0; i < numCities; i++) {
        if (i != city && dist[city][i] < minDist) {
            nearestCity = i;
            minDist = dist[city][i];
        }
    }

    return nearestCity;
}
