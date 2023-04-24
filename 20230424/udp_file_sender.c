#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // argv[1]: dia chi IP
    // argv[2]: cong
    // argv[3]: file can chuyen
    
    if (argc != 4)
    {
        printf("Sai so luong tham so.\n");
        return 1;
    }

    // Tao UDP socket
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Khai bao dia chi ben nhan
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);
    
    // Doc noi dung file va gui di
    // Gửi tin nhắn
    char *filename = "ecard.mp4";
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("Khong the mo file.\n");
        return 1;
    }

    // Lay kich thuoc file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buf[2048];

    // Gui msg chua kich thuoc file va ten file
    memcpy(buf, &fsize, sizeof(long));
    strcpy(buf + sizeof(long), filename);
    int nbytes = sizeof(long) + strlen(filename) + 1;
    sendto(sender, buf, nbytes, 0, (struct sockaddr *)&addr, sizeof(addr));

    // Doc noi dung file va gui sang server
    while (!feof(f))
    {
        int len = fread(buf, 1, sizeof(buf), f);
        if (len <= 0)
            break;
        sendto(sender, buf, len, 0, (struct sockaddr *)&addr, sizeof(addr));
    }
    
    close(sender);
    fclose(f);
    return 0;
}