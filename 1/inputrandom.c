#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

int main() {
    int upper;
    int lower;
    bool ok;

    do
    {
        ok=true;
        printf("Please give the lower and the upper boundary: ");
        if(scanf("%d, %d", &lower, &upper) != 2 || upper<lower)
        {
            ok=false;
            printf("Incorrect input!\n");
            while(getchar()!='\n');
        }
    } while (!ok);

    srand(time(0));

    printf("Random integer: %d\n", (rand()%(upper-lower)+1)+lower);
    printf("Random float: %f", ((upper - lower)*((float)rand()/RAND_MAX))+lower);
    return 0;
}