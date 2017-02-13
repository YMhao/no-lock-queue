#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "timer.h"

int mutex = 0;
int lock = 0;
int unlock = 1;

static volatile int count = 0;
void *test_func(void *arg)
{
    int i = 0;
    for(i = 0; i < 2000000; i++)
    {
        while (!(__sync_bool_compare_and_swap (&mutex,lock, 1) ))usleep(100000);
        count++;
        __sync_bool_compare_and_swap (&mutex, unlock, 0);
    }
    return NULL;
}

int main(int argc, const char *argv[])
{
    Timer timer;
    timer.Start();
    pthread_t thread_ids[10];
    int i = 0;

    for(i = 0; i < sizeof(thread_ids)/sizeof(pthread_t); i++)
    {
        pthread_create(&thread_ids[i], NULL, test_func, NULL);
    }

    for(i = 0; i < sizeof(thread_ids)/sizeof(pthread_t); i++)
    {
        pthread_join(thread_ids[i], NULL);
    }

    timer.Stop();
    timer.Cost_time();
    printf("结果:count = %d\n",count);

    return 0;
}
