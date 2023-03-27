#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char str[] = "227 Entering Passive Mode (213,229,112,130,216,4)";

    int i1, i2, i3, i4;
    int p1, p2;

    char *pc = strchr(str, '(');

    char *p = strtok(pc, "(),");
    printf("%s\n", p);
    i1 = atoi(p);
    p = strtok(NULL, "(),");
    i2 = atoi(p);
    printf("%s\n", p);
    p = strtok(NULL, "(),");
    i3 = atoi(p);
    printf("%s\n", p);
    p = strtok(NULL, "(),");
    i4 = atoi(p);
    printf("%s\n", p);


    p = strtok(NULL, "(),");
    p1 = atoi(p);
    printf("%s\n", p);
    p = strtok(NULL, "(),");
    p2 = atoi(p);
    printf("%s\n", p);

    printf("IP: %u.%u.%u.%u:%u\n", i1, i2, i3, i4, p1 * 256 + p2);
}