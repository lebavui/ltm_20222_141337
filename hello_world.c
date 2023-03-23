#include <stdio.h>

int main()
{
    printf("Hello World.\n");
    int sum;
    for (int i = 1; i <= 10; i++)
    {
        sum += i;
        printf("%d\n", sum);
    }
}