#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        printf("Sai tham so URL.\n");
        return 1;
    }

    // Khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Xu ly tham so argv[1]
    // Tach ra ten mien
    char *p = strchr(argv[1] + 7, '/');
    char *domain = NULL;
    char *path = NULL;

    int len;
    if (p == NULL)
    {
        len = strlen(argv[1]) - 7;
        domain = malloc(len + 1);
        memcpy(domain, argv[1] + 7, len);
        domain[len + 1] = 0;
        path = malloc(2);
        strcpy(path, "/");
    }
    else
    {
        len = (p - argv[1]) - 7;
        domain = malloc(len + 1);
        memcpy(domain, argv[1] + 7, len);
        domain[len + 1] = 0;
        int path_size = strlen(argv[1]) - len - 7 + 1;
        path = malloc(path_size);
        memcpy(path, argv[1] + 7 + len, path_size);
        path[path_size] = 0;    
    }

    printf("domain: %s\n", domain);
    printf("path: %s\n", path);

    // Tach ra duong dan

    // Phan giai dia chi
    struct addrinfo *result;
    int ret = getaddrinfo(domain, "http", NULL, &result);
    if (ret != 0 || result == NULL)
    {
        printf("Khong phan giai duoc ten mien.\n");
        return 1;
    }

    // Ket noi den server
    ret = connect(client, result->ai_addr, result->ai_addrlen);
    if (ret == -1) {
        printf("Khong ket noi duoc den server!");
        return 1;
    }
        
    // Gui tin nhan den server
    char msg[256];
    sprintf(msg, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, domain);
    send(client, msg, strlen(msg), 0);

    free(domain);
    free(path);

    // Nhan tin nhan tu server
    char buf[2048];
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s", buf);
    }
    
    // Ket thuc, dong socket
    close(client);

    return 0;
}