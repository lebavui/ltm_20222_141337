#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void signalHandler(int signo)
{
    int pid = wait(NULL);
    printf("Child %d terminated.\n", pid);
}

int main()
{
    signal(SIGCHLD, signalHandler);
    
    if (fork() == 0)
    {
        printf("Child process started.\n");
        sleep(10);
        printf("Child process done.\n");
        exit(0);
    }

    printf("Press ENTER to exit...\n");
    getchar();
    return 0;
}