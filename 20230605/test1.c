#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_proc(void *);

int main()
{
    pthread_t thread_id;
    int client = 1234;
    if (pthread_create(&thread_id, NULL, thread_proc, &client))
    {
        printf("pthread_create() failed.\n");
        return 1;
    }

    printf("Main thread.\n");

    pthread_join(thread_id, NULL);
    printf("Child thread done.\n");
    
    return 0;
}

void *thread_proc(void *param)
{
    int *pclient = (int *)param;
    int client = *(int *)param;

    printf("*pclient = %d --- client = %d\n", *pclient, client);

    for (int i = 0; i < 10; i++)
    {
        printf("Child thread running.\n");
        sleep(1);
    }

    printf("Child thread %ld\n", pthread_self());
    return NULL;
}