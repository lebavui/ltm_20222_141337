#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    // Chuyển socket sang chế độ bất đồng bộ
    unsigned long ul = 1;
    ioctl(listener, FIONBIO, &ul);

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

    int clients[64];
    int num_clients = 0;

    char buf[256];

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                // Lỗi do thao tác vào ra chưa hoàn tất (vẫn đang chờ kết nối mới)
                // Bỏ qua xử lý socket khác
            }
            else
            {
                perror("accept() failed");
                return 1;
            }
        }
        else
        {
            printf("New client connected: %d\n", client);
            clients[num_clients++] = client;
            unsigned long ul = 1;
            ioctl(client, FIONBIO, &ul);
        }

        for (int i = 0; i < num_clients; i++)
        {
            int ret = recv(clients[i], buf, sizeof(buf), 0);
            if (ret == -1)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    // Lỗi do thao tác vào ra chưa hoàn tất (vẫn đang chờ dữ liệu)
                    // Bỏ qua xử lý socket khác
                }
                else
                {
                    close(clients[i]);
                    continue;
                }
            }
            else if (ret == 0)
            {
                close(clients[i]);
                // TODO: Xóa socket ra khỏi mảng
                continue;
            }
            else
            {
                buf[ret] = 0;
                printf("Du lieu tu client %d: %s\n", clients[i], buf);
            }
        }
    }
    
    close(listener);    

    return 0;
}