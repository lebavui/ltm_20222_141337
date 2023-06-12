#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

void *thread_proc(void *);

int main() 
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000); 

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect() failed");
        return 1;
    }
    
    while (1)
    {
        // Nhap username
        char username[64];
        printf("Nhap tai khoan de dang nhap: ");
        fgets(username, sizeof(username), stdin);

        char buf[256];
        sprintf(buf, "client_id: %s", username);
        send(client, buf, strlen(buf), 0);

        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            close(client);
            return 1;
        } 

        printf("Received: %s\n", buf);

        if (strncmp(buf, "OK", 2) == 0)
        {
            printf("Dang nhap thanh cong.\n");
            break;
        }
        else
        {
            printf("Dang nhap that bai.\n");
        }
    }
    
    pthread_t tid;
    pthread_create(&tid, NULL, thread_proc, &client);

    char buf[256];
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, strlen(buf), 0);
        if (strncmp(buf, "exit", 4) == 0)
            break;
    }

    // Ket thuc, dong socket
    close(client);

    return 0;
}

void *thread_proc(void *param)
{
    int client = *(int *)param;
    char buf[256];

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        buf[ret] = 0;
        printf("Received: %s\n", buf);
    }

    close(client);
}