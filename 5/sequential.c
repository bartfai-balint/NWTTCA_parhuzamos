#include <stdio.h>
#include<stdlib.h>
#include<time.h>

void sequential(double nums[], long n);

int main(){
    long n = 1e8;
    double* randNums;
    randNums = (double*) malloc(n * sizeof(double));

    srand(time(0));

    clock_t begin = clock();

    for(int i = 0; i < n; i++)
    {
        randNums[i]=(double) rand()/RAND_MAX;
    }

    sequential(randNums, n);

    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime with sequential: %f\n", timeSpent);
    
    return 0;
}

void sequential(double nums[], long n)
{
    double product=1;

    for (int i = 0; i < n; i++)
    {
        product *= nums[i];
    }

    printf("\nProduct with sequential: %lf", product);
    
}