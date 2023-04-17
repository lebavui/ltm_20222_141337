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
    addr.sin_port = htons(9090); 

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect() failed");
        return 1;
    }
        
    // Truyen nhan du lieu
    char buf[256];
    char computer_name[64];

    // Nhap ten may tinh
    printf("Nhap ten may tinh: ");
    scanf("%s", computer_name);

    // Xoa \n trong bo dem
    getchar();

    // Chuyen vao buffer
    strcpy(buf, computer_name);
    int pos = strlen(computer_name);
    buf[pos] = 0;
    pos++;

    int n;
    printf("Nhap so luong o dia:\n");
    scanf("%d", &n);

    // Xoa \n trong bo dem
    getchar();

    char disk_letter;
    short int disk_size;

    for (int i = 0; i < n; i++)
    {
        printf("Nhap ten o dia: ");
        scanf("%c", &disk_letter);
        
        printf("Nhap kich thuoc o dia: ");
        scanf("%hd", &disk_size);
        
        // Xoa \n trong bo dem
        getchar();
        
        // Chuyen vao buffer
        buf[pos] = disk_letter;
        pos++;
        memcpy(buf + pos, &disk_size, sizeof(disk_size));
        pos += sizeof(disk_size);
    }

    printf("Buffer size: %d\n", pos);
    send(client, buf, pos, 0);

    // Ket thuc, dong socket
    close(client);

    return 0;
}