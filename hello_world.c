#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
    if (argc != 2)
    {
        printf("Nhap sai tham so.\n");
        return 1;
    }
        
    int n = atoi(argv[1]);
    for (int i = 0; i < n; i++)
        printf("%d\n", i);
}