#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

void *client_thread(void *);

void signal_handler(int signo)
{
    wait(NULL);
}

int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        return 1;
    }

    signal(SIGPIPE, signal_handler);

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            perror("accept() failed");
            continue;;
        }
        printf("New client connected: %d\n", client);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_thread, &client);
        pthread_detach(thread_id);
    }
    
    close(listener);    

    return 0;
}

void *client_thread(void *param)
{
    int client = *(int *)param;
    char buf[2048];
    
    int ret = recv(client, buf, sizeof(buf) - 1, 0);
    if (ret <= 0)
        return NULL;
    
    buf[ret] = 0;
    printf("Received from %d: %s\n", client, buf);
    
    char method[16];
    char path[256];
    sscanf(buf, "%s %s", method, path);

    if (strcmp(path, "/web") == 0)
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);
        char *response_body = "<html><body><h1>This is a web page.</h1></body></html>";
        send(client, response_body, strlen(response_body), 0);
    }
    else if (strcmp(path, "/photo") == 0)
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nConnection: close\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);

        FILE *f = fopen("test.jpg", "rb");
        while (1)
        {
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else if (strcmp(path, "/audio") == 0)
    {
        FILE *f = fopen("test.mp3", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char response_header[2048];
        sprintf(response_header, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: audio/mp3\r\nConnection: keep-alive\r\n\r\n", fsize);
        send(client, response_header, strlen(response_header), 0);

        while (1)
        {
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);
        char *response_body = "<html><body><h1>Hello World</h1></body></html>";
        send(client, response_body, strlen(response_body), 0);
    }

    close(client);
}