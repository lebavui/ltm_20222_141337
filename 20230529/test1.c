#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("Before FORK\n");
    for (int i = 0; i < 5; i++)
        if (fork() == 0)
        {
            printf("Child process\n");
            exit(0);    
        }
    printf("Main process\n");
    return 0;
}