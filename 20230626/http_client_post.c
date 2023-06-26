#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    // http://httpbin.org/post
    
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct addrinfo *info;
    int ret = getaddrinfo("httpbin.org", "http", NULL, &info);
    if (ret != 0 || info == NULL)
    {
        printf("Khong phan giai duoc ten mien.\n");
        exit(1);
    }

    if (connect(client, info->ai_addr, info->ai_addrlen)) {
        perror("connect() failed");
        exit(1);
    }
        
    // Gui yeu cau den server
    char *request = "POST /post HTTP/1.1\r\nHost: httpbin.org\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\n";
    send(client, request, strlen(request), 0);
    char *body = "param1=value1&param2=value2";
    send(client, body, strlen(body), 0);

    // Nhan phan hoi tu server
    char buf[2048];
    int len = recv(client, buf, sizeof(buf) - 1, 0);
    buf[len] = 0;

    puts(buf);

    // Ket thuc, dong socket
    close(client);

    return 0;
}