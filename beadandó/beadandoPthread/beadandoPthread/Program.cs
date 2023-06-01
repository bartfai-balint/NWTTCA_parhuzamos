using System;
using System.Collections.Generic;
using System.Threading.Tasks;

class Program
{
    static int numOfCities = 5; 
    static int numOfAnts = 10; 
    static int numOfIterations = 100; 

    static double alpha = 1.0; 
    static double beta = 5.0; 
    static double evaporationRate = 0.5; 
    static double Q = 100.0; 

    static double[,] distanceMatrix;

    static Random random = new Random();

    static int[] bestTour; 
    static double bestTourLength; 

    static object lockObject = new object();

    static void Main(string[] args)
    {
        InitializeDistanceMatrix();

        bestTourLength = double.MaxValue;

        Parallel.For(0, numOfIterations, i =>
        {
            int[] tour = AntColonyOptimization();

            lock (lockObject)
            {
                double tourLength = GetTourLength(tour);
                if (tourLength < bestTourLength)
                {
                    bestTourLength = tourLength;
                    bestTour = tour;
                }
            }
        });

        Console.WriteLine("Legjobb útvonal: " + string.Join(" -> ", bestTour));
        Console.WriteLine("Legjobb útvonal hossza: " + bestTourLength);

        Console.ReadLine();
    }

    static int[] AntColonyOptimization()
    {
        int[] tour = new int[numOfCities];

        int startCity = random.Next(0, numOfCities);
        tour[0] = startCity;

        bool[] visitedCities = new bool[numOfCities];
        visitedCities[startCity] = true;

        for (int i = 1; i < numOfCities; i++)
        {
            int currentCity = tour[i - 1];
            int nextCity = ChooseNextCity(currentCity, visitedCities);
            tour[i] = nextCity;
            visitedCities[nextCity] = true;
        }

        return tour;
    }

    static int ChooseNextCity(int currentCity, bool[] visitedCities)
    {
        double[] probabilities = new double[numOfCities];
        double sum = 0.0;

        for (int i = 0; i < numOfCities; i++)
        {
            if (!visitedCities[i])
            {
                double pheromone = Math.Pow(1.0 / distanceMatrix[currentCity, i], beta);
                probabilities[i] = Math.Pow(pheromone, alpha);
                sum += probabilities[i];
            }
        }

        double randomValue = random.NextDouble() * sum;

        double cumulative = 0.0;
        for (int i = 0; i < numOfCities; i++)
        {
            if (!visitedCities[i])
            {
                cumulative += probabilities[i];
                if (cumulative >= randomValue)
                    return i;
            }
        }

        return -1; 
    }

    static double GetTourLength(int[] tour)
    {
        double tourLength = 0.0;

        for (int i = 0; i < numOfCities - 1; i++)
        {
            int city1 = tour[i];
            int city2 = tour[i + 1];
            tourLength += distanceMatrix[city1, city2];
        }

        int lastCity = tour[numOfCities - 1];
        int firstCity = tour[0];
        tourLength += distanceMatrix[lastCity, firstCity];

        return tourLength;
    }

    static void InitializeDistanceMatrix()
    {
        distanceMatrix = new double[,]
        {
            { 0.0, 10.0, 15.0, 20.0, 25.0 },
            { 10.0, 0.0, 35.0, 25.0, 20.0 },
            { 15.0, 35.0, 0.0, 30.0, 40.0 },
            { 20.0, 25.0, 30.0, 0.0, 15.0 },
            { 25.0, 20.0, 40.0, 15.0, 0.0 }
        };
    }
}