#include <stdio.h>
#include <time.h>

void primeCounter(int n);

int main() {
    int n;

    printf("Adja meg a felso hatart: ");
    scanf("%d", &n);

    clock_t begin = clock();
    primeCounter(n);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f", time_spent);
    return 0;
}

void primeCounter(int n)
{

    int i, j, flag = 0, count = 0;

    for(i = 2; i <= n; i++)
    {
        for (j = 2; j <= i / 2; j++)
        {
            if (i % j == 0)
            {
                flag = 1;
                break;
            }
        }

        if(flag == 0)
        {
            count++;
            printf("%d ", i);
        }

        flag = 0;
    }

    printf("Primek szama: %d\n", count);
}
