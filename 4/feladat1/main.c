#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void Upload(int *array, int n);
int Odd( int *array, int n);    //páratlan
int Even(int *array, int n);
int Null(int *array, int n);
int Negative(int *array, int n);
int Intervall(int *array, int n, int from, int until);
void runtime_display(clock_t sequential_time, clock_t pthread_time, int size);


int main()
{
    int i;
    for(i=100;i<100000;i+=100)
    {
    int n=i;
    int array[n];
    int even;
    int odd;
    int null;
    int negative;
    int from=10, until=100;
    int intervall;

    clock_t sequential_time1;
    sequential_time1=clock();
    Upload(array,n);
    even=Even(array,n);
    odd=Odd(array,n);
    printf("\nA paros szamok: %d, paratlan szamok: %d.",even,odd);
    null=Null(array,n);
    printf("\nA 0-ak szama: %d.",null);
    negative=Negative(array,n);
    printf("\nA negatív szmok száma: %d.",negative);
    intervall=Intervall(array,n,from,until);
    printf("\nAz %d-%d intervallumba eső elemek száma: %d.",from, until, intervall);
    clock_t sequential_time2;
    sequential_time2=clock();
    clock_t sequential_time;
    sequential_time=sequential_time2-sequential_time1;

    clock_t pthread_time1;
    pthread_time1=clock();
    pthread_create(array,NULL,Upload,array,n);
    pthread_create(even,NULL,Even,array,n);
    pthread_create(odd,NULL,Odd,array,n);
    pthread_create(null,NULL,Null,array,n);
    pthread_create(negative,NULL,Negative,array,n);
    pthread_create(intervall,NULL,Intervall,array,n,from,until);
    clock_t pthread_time2;
    pthread_time2=clock();
    clock_t pthread_time;
    pthread_time=pthread_time2-pthread_time1;

    runtime_display(sequential_time,pthread_time,n);
    }
    return 0;
}

void Upload( int* array, int n)
{
    srand(time(NULL));
    int i;
    int ok=0;
    for(i=0; i<n; i++)
    { 
        array[i] = rand();
    }
    return;
}

int Odd( int *array, int n)
{
    int odd=0;
    int i;
    for(i=0; i<n; i++)
    {
        if(array[i]%2 != 0)
        {
            odd++;
        }
    }
    return odd;
}    //páratlan
int Even(int *array, int n)
{
    int even=0;
    int i;
    for(i=0; i<n; i++)
    {
        if(array[i]%2==0)
        {
            even++;
        }
    }
    return even;
}
int Null(int *array, int n)
{
    int null=0;
    int i;
    for(i=0; i<n; i++)
    {
        if(array[i]==0)
        {
            null++;
        }
    }
    return null;
}
int Negative(int *array, int n)
{
    int negative=0;
    int i;
    for(i=0; i<n; i++)
    {
        if(array[i]<0)
        {
            negative++;
        }
    }
    return negative;
}
int Intervall(int *array, int n, int from, int until)
{
    int intervall=0;
    int i;
    for(i=0;i<n;i++)
    {
        if(array[i]>=from || array[i]<=until)
        {
            intervall++;
        }
    }
    return intervall;
}
void runtime_display(clock_t sequential_time, clock_t pthread_time, int size)
{
    FILE *fp;
    int i;
   
    fp=fopen("runtime_alg_par_2.txt","a");
    if(fp==NULL)
    {
        printf("\nSikertelen fájl megnyitás");
    }
    else
    {
        fprintf(fp,"\n%d; %.2lf; %.2lf;\n",size, sequential_time, pthread_time);
        fclose(fp);
    }
    return;
}