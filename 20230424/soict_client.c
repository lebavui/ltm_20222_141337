#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("connect() failed");
        return 1;
    }

    FILE *f = fopen("test.txt", "rb");
    char buf[256];
    while (!feof(f))
    {
        int ret = fread(buf, 1, sizeof(buf), f);
        if (ret <= 0)
            break;
        send(client, buf, ret, 0);
    }
    fclose(f);
    close(client);
}