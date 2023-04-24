#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

    int client = accept(listener, NULL, NULL);

    // Khai báo chuỗi cần tìm
    char *find = "0123456789";
    int len = strlen(find) - 1; // Độ dài chuỗi đệm
    char buf[265]; // 256 + 9
    
    // Khởi tạo chuỗi đệm chứa dấu cách
    memset(buf, 32, len);

    int count = 0;

    while (1)
    {
        // Nhận dữ liệu mới vào vị trí sau chuỗi đệm
        int ret = recv(client, buf + len, sizeof(buf) - len, 0);
        if (ret <= 0)
            break;
        
        // Đếm số lần suất hiện trong buffer chứa dữ liệu nhận được
        char *p = strstr(buf, find);
        while (p != NULL && (p - buf) < ret)
        {
            count++;
            p = strstr(p + 1, find);
        }

        // Cập nhật chuỗi đệm chứa các ký tự cuối của buffer hiện tại
        memcpy(buf, buf + ret, len);
    }

    printf("Number of sub string: %d\n", count);

    close(client);
    close(listener);
}