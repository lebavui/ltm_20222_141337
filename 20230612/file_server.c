#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

void signalHandler(int signo)
{
    int pid = wait(NULL);
    printf("Child %d terminated.\n", pid);
}

int main(int argc, char *argv[]) 
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

    signal(SIGCHLD, signalHandler);

    while (1)
    {
        printf("Waiting for new client...\n");
        int client = accept(listener, NULL, NULL);
        if (fork() == 0)
        {
            // Tien trinh con
            close(listener);

            // Gui danh sach file cho client
            DIR *dir = opendir(argv[1]);
            struct dirent *entry;
    
            char *file_list = malloc(1);
            file_list[0] = '\0';

            int num_files = 0;
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_type == DT_REG)
                {
                    num_files++;
                    file_list = realloc(file_list, strlen(file_list) + strlen(entry->d_name) + 3);
                    strcat(file_list, entry->d_name);
                    strcat(file_list, "\r\n");
                }
            }

            closedir(dir);

            if (num_files == 0)
            {
                char *msg = "ERROR No file to download.\n";
                send(client, msg, strlen(msg), 0);
                close(client);
                exit(0);
            }
            else
            {
                char buf[256];
                sprintf(buf, "OK %d\r\n", num_files);
                send(client, buf, strlen(buf), 0);
                send(client, file_list, strlen(file_list), 0);
                send(client, "\r\n", 2, 0);
            }

            // Xu ly ket noi tu client

            char buf[256];
            while (1)
            {
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    break;
                
                if (buf[ret - 1] == '\n')
                    buf[ret - 1] = 0;
                else
                    buf[ret] = 0;
                
                printf("Received from %d: %s\n", client, buf);

                FILE *f = fopen(buf, "rb");

                if (f == NULL)
                {
                    // File khong mo duoc. Gui thong bao loi.
                    char *msg = "ERROR Cannot open file.\n";
                    send(client, msg, strlen(msg), 0);
                }
                else
                {
                    // Gui noi dung file cho client
                    fseek(f, 0, SEEK_END);
                    long file_size = ftell(f);
                    fseek(f, 0, SEEK_SET);

                    sprintf(buf, "OK %ld\r\n", file_size);
                    send(client, buf, strlen(buf), 0);

                    while (1)
                    {
                        ret = fread(buf, 1, sizeof(buf), f);
                        if (ret <= 0)
                            break;
                        send(client, buf, ret, 0);
                    }

                    fclose(f);
                    break;
                }
            }

            close(client);
            exit(0);
        }

        // Tien trinh cha
        close(client);
    }
    
    close(listener);    

    return 0;
}