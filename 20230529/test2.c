#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    for (int i = 0; i < 5; i++)
        if (fork() == 0)
        {
            printf("Child process started\n");
            sleep(10);
            printf("Child process done\n");
            exit(0);    
        }

    // tiến trình cha
    printf("Waiting for the child process\n");
    int pid;
    while ((pid = wait(NULL)) != -1)
    {
        printf("Child %d terminated.\n", pid);
    }
    printf("Main process done\n"); 
    return 0;
}