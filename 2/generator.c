#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

void randomGenerator(int n, int lower, int upper);

int main() {
    int n=10;
    randomGenerator(n, 500, 1000);

    return 0;
}

void randomGenerator(int n, int lower, int upper)
{
    FILE *fptr;
    fptr = fopen("numbers.txt","w");

    if(fptr == NULL)
    {
      printf("Error!");
      exit(1);
    }

    int array[n];
    int i;
    for(i=0;i<=n;i++)
    {
        array[i] = (rand() %(upper - lower + 1)) + lower;
        fprintf(fptr, "%d: %d\n", i+1, array[i]);
        printf( "%d: %d\n", i, array[i]);
    }

    fclose(fptr);
}