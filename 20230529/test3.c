#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main()
{
    for (int i = 0; i < 5; i++)
        if (fork() == 0)
        {
            // Child process
            while (1)
            {
                printf("Child process running.\n");
                sleep(1);
            }
            exit(0);
        }

    // Main process
    printf("Main process started.\n");
    sleep(10);
    printf("Main process done.\n");

    killpg(0, SIGKILL);
    return 0;
}