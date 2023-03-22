#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *newThread(void *vargp)
{
    printf("Thread\n");
    printf("4 mp...\n");
	sleep(4);
	return NULL;
}

int main()
{
	pthread_t thread_id;
	printf("8 mp...\n");
    sleep(8);
	pthread_create(&thread_id, NULL, newThread, NULL);
	pthread_join(thread_id, NULL);
	printf("Main\n");
	exit(0);
}
