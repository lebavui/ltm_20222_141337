#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 8

double local_sum[NUM_THREADS];
static long num_steps = 100000;
double step;

void *thread_proc(void *);

int main()
{
    step = 1.0 / num_steps;
    pthread_t thread_ids[NUM_THREADS];

    int params[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        params[i] = i;
        local_sum[i] = 0;
    }

    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&thread_ids[i], NULL, thread_proc, &params[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(thread_ids[i], NULL);

    double total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        total_sum += local_sum[i];

    clock_gettime(CLOCK_REALTIME, &stop);
    double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3; // in microseconds

    printf("Elapsed time: %f us\n", result);

    double pi = total_sum * step;
    printf("PI = %.10f\n", pi);
    
    return 0;
}

void *thread_proc(void *param)
{
    int idx = *(int *)param;
    double x, sum = 0;

    printf("Thread index: %d\n", idx);

    for (int i = idx; i < num_steps; i += NUM_THREADS)
    {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    local_sum[idx] = sum;
}