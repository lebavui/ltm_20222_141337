#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <malloc.h>

int main() {
    // http://api.weatherapi.com/v1/current.json?key=48bab0abac324847925230945232306&q=Hanoi&aqi=no
    
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct addrinfo *info;
    int ret = getaddrinfo("api.weatherapi.com", "http", NULL, &info);
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
    char *request = "GET /v1/current.json?key=48bab0abac324847925230945232306&q=Hanoi&aqi=no HTTP/1.1\r\nHost: api.weatherapi.com\r\nAccept: */*\r\nConnection: close\r\n\r\n";
    send(client, request, strlen(request), 0);

    // Nhan phan hoi tu server
    char buf[2048];
    char *response = NULL;
    int size  = 0;

    while (1)
    {
        int len = recv(client, buf, sizeof(buf), 0);
        if (len <= 0)
            break;
        
        response = realloc(response, size + len);
        memcpy(response + size, buf, len);
        size += len;
    }

    response = realloc(response, size + 1);
    size += 1;
    response[size - 1] = 0;

    puts(response);
    
    char *pos1 = strstr(response, "temp_c");
    if (pos1 != NULL)
    {
        pos1 = strchr(pos1, ':') + 1;
        char *pos2 = strchr(pos1, ',');
        int value_length = pos2 - pos1;
        memcpy(buf, pos1, value_length);
        buf[value_length] = 0;
        double temp = atof(buf);
        printf("Temp: %f\n", temp);
    }

    return 0;
}