#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000); 

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect() failed");
        return 1;
    }
        
    char buf[256];

    while (1)
    {
        // Doc du lieu tu ban phim
        printf("Nhap xau: ");
        fgets(buf, sizeof(buf), stdin);

        // Truyen sang server
        send(client, buf, strlen(buf), 0);

        // Nhan du lieu tu server
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        
        // In ra man hinh
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
    }

    // Ket thuc, dong socket
    close(client);

    return 0;
}