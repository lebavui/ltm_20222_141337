#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct client_info 
{
    char client_addr_str[24];   // Luu tru dia chi client IP:Port
    char filename[256];     // Luu tru ten file can ghi
    long filesize;          // Kich thuoc file
    FILE *file;
    int total_count;
    int current_count;
};

struct client_info clients[32];
int num_clients = 0;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Sai so luong tham so.\n");
        return 1;
    }

    // Tao UDP socket
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Khai bao dia chi ben nhan
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    char buf[2048];
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    char client_addr_str[24];

    while (1)
    {
        int ret = recvfrom(receiver, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        
        // Tao chuoi dia chi duy nhat cua client
        sprintf(client_addr_str, "%s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Kiem tra client da duoc luu trong danh sach chua
        // Neu co => Ghi du lieu vao file
        // Neu chua => Tao doi tuong moi
        int i = 0;
        for (; i < num_clients; i++)
            if (strcmp(clients[i].client_addr_str, client_addr_str) == 0)
                break;
        
        if (i == num_clients)
        {
            // Chua co trong danh sach
            struct client_info new_client;
            strcpy(new_client.client_addr_str, client_addr_str);
            memcpy(&new_client.filesize, buf, sizeof(new_client.filesize));
            strcpy(new_client.filename, "new_");
            strcat(new_client.filename, buf + sizeof(new_client.filesize));
            new_client.file = fopen(new_client.filename, "wb");
            clients[num_clients] = new_client;
            num_clients++;

            printf("Start receiving %s from %s\n", new_client.filename, new_client.client_addr_str);
        }
        else
        {
            // Co roi, ghi du lieu vao file
            fwrite(buf, 1, ret, clients[i].file);
            if (ftell(clients[i].file) == clients[i].filesize)
            {
                fclose(clients[i].file);

                printf("Finish receiving %s from %s\n", clients[i].filename, clients[i].client_addr_str);

                // Da ghi xong, xoa khoi mang
                if (i < num_clients - 1)
                    clients[i] = clients[num_clients - 1];
                num_clients--;
            }         
        }
    }
     
    return 0;
}