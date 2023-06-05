#include <stdio.h>
#include <pthread.h>

#define NLOOP 100
int counter = 0;

pthread_mutex_t counter_m = PTHREAD_MUTEX_INITIALIZER;

void * thread_proc(void *);

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_proc, NULL);
    pthread_create(&t2, NULL, thread_proc, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}

void *thread_proc(void *param)
{
    for (int i = 0; i < NLOOP; i++)
    {
        pthread_mutex_lock(&counter_m);
        int val = counter;
        printf("Thread %ld: %d\n", pthread_self(), val + 1);
        counter = val + 1;        
        pthread_mutex_unlock(&counter_m);
    }
}