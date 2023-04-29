#include <stdio.h>
#include<stdlib.h>
#include<time.h>

double recursive(double nums[], int left, long right, int depth);

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

    double product = recursive(randNums, 0, n-1, 0);
    printf("\nProduct with recursive: %.2f", product);

    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime with recursive: %f", timeSpent);
    
    
    return 0;
}

double recursive(double nums[], int left, long right, int depth)
{
    if (left == right)
        return nums[left];

    long mid = (left + right) / 2;
    double leftProduct = recursive(nums, left, mid, depth+1);
    double rightProduct = recursive(nums, mid+1, right, depth+1);

    return leftProduct * rightProduct;
}