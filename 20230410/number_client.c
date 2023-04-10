#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Client nhập số từ bàn phím và gửi sang server

int main()
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    float arr[100];
    int n = 64;
    while (1)
    {
        printf("Nhap so phan tu: ");
        scanf("%d", &n);
        if (n == 0)
            break;
        
        // Khoi tao mang
        for (int i = 0; i < n; i++)
            arr[i] = i;

        // Gui mang sang server
        send(client, arr, sizeof(float) * n, 0);
    }
    
    close(client);
}