#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/resource.h>

void* thread_proc(void *arg);

int main() 
{
    // Tạo socket chờ kết nối
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Khai báo cấu trúc địa chỉ server chờ ở cổng 9000
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gắn địa chỉ với socket và chuyển sang trạng thái chờ kết nối
    bind(listener, (struct sockaddr *)&addr, sizeof(addr));
    listen(listener, 5);

    pthread_t thread_id;

    while (1)
    {
        printf("Waiting for new client ...\n");
        // Chấp nhận kết nối mới
        int client = accept(listener, NULL, NULL);
        if (client == -1)
            continue;
        printf("New client connected: %d\n", client);
    
        // Tạo luồng để xử lý yêu cầu từ client
        int ret = pthread_create(&thread_id, NULL, thread_proc, (void *)&client);
        if (ret != 0) 
        {
            printf("Could not create thread!\n");   
        }

        // Yêu cầu luồng tự giải phóng khi kết thúc
        pthread_detach(thread_id);
        // Ưu tiên luồng mới tạo bắt đầu chạy
        sched_yield();
    }

    return 0;
}

void* thread_proc(void *arg)
{
    printf("child thread created.\n");
    
    int client = *(int *)arg;
    char buf[2048];

    int len = recv(client, buf, sizeof(buf) - 1, 0);

    buf[len] = 0;

    puts(buf);
    
    // process request
    if (strncasecmp(buf, "GET /calc", 9) == 0)
    {
        // extract query string
        char *pos1 = strchr(buf, '?') + 1;
        char *pos2 = strchr(pos1, ' ');
        int query_length = pos2 - pos1;
        char *query = malloc(query_length + 1);
        memcpy(query, pos1, query_length);
        query[query_length] = 0;

        // get parameter from query string
        float a, b, result;
        int cmd;
        
        char *p = strtok(query, "&");
        while (p != NULL)
        {
            if (strncmp(p, "a=", 2) == 0)
            {
                a = atof(p + 2);
            }
            else if (strncmp(p, "b=", 2) == 0)
            {
                b = atof(p + 2);
            }
            else if (strncmp(p, "cmd=", 4) == 0)
            {
                if (strcmp(p + 4, "add") == 0)
                {
                    cmd = 1;
                }
                else if (strcmp(p + 4, "sub") == 0)
                {
                    cmd = 2;
                }
                else if (strcmp(p + 4, "mul") == 0)
                {
                    cmd = 3;
                }
                if (strcmp(p + 4, "div") == 0)
                {
                    cmd = 4;
                }
            }
            p = strtok(NULL, "&");
        }

        if (cmd == 1)
        {
            result = a + b;
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f+%f=%f</body></html>", a, b, result);
        }
        else if (cmd == 2)
        {
            result = a - b;
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f-%f=%f</body></html>", a, b, result);
        }
        else if (cmd == 3)
        {
            result = a * b;
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f*%f=%f</body></html>", a, b, result);
        }
        else if (cmd == 4)
        {
            if (b == 0)
            {
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>Devide by zero.</body></html>");
            }
            else
            {
                result = a / b;
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f/%f=%f</body></html>", a, b, result);
            }
        }

        // send result to client
        send(client, buf, strlen(buf), 0);
    }
    else if (strncasecmp(buf, "POST /calc", 10) == 0)
    {
        // extract query string
        char *pos = strstr(buf, "\r\n\r\n") + 4;
        int body_length = strlen(buf) - (pos - buf);
        char *body = malloc(body_length + 1);
        memcpy(body, pos, body_length);
        body[body_length] = 0;

        // get parameter from query string
        float a, b, result;
        int cmd;
        
        char *p = strtok(body, "&");
        while (p != NULL)
        {
            if (strncmp(p, "a=", 2) == 0)
            {
                a = atof(p + 2);
            }
            else if (strncmp(p, "b=", 2) == 0)
            {
                b = atof(p + 2);
            }
            else if (strncmp(p, "cmd=", 4) == 0)
            {
                if (strcmp(p + 4, "add") == 0)
                {
                    cmd = 1;
                }
                else if (strcmp(p + 4, "sub") == 0)
                {
                    cmd = 2;
                }
                else if (strcmp(p + 4, "mul") == 0)
                {
                    cmd = 3;
                }
                if (strcmp(p + 4, "div") == 0)
                {
                    cmd = 4;
                }
            }
            p = strtok(NULL, "&");
        }

        if (cmd == 1)
        {
            result = a + b;
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f+%f=%f</body></html>", a, b, result);
        }
        else if (cmd == 2)
        {
            result = a - b;
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f-%f=%f</body></html>", a, b, result);
        }
        else if (cmd == 3)
        {
            result = a * b;
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f*%f=%f</body></html>", a, b, result);
        }
        else if (cmd == 4)
        {
            if (b == 0)
            {
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>Devide by zero.</body></html>");
            }
            else
            {
                result = a / b;
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>%f/%f=%f</body></html>", a, b, result);
            }
        }

        // send result to client
        send(client, buf, strlen(buf), 0);
    }
    else
    {
        
    }
    

    close(client);

    printf("child thread finished.\n");
}