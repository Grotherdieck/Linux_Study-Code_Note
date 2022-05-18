#include "comm.h"

int main()
{
    umask(0);
    if (mkfifo(MY_FIFO, 0666) < 0)
    {
        perror("mkfifo fail\n");
        exit(1);
    }

    // 假设让sever去读
    int fd = open(MY_FIFO, O_RDONLY);
    if (fd < 0)
    {
        perror("open fifo fail\n");
        exit(2);
    }

    // 业务逻辑
    while (1)
    {
        // sleep(50);
        char buffer[64] = {0};
        ssize_t s = read(fd, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            // success
            buffer[s] = 0;
            if (strcmp(buffer, "show") == 0)
            {
                // 创建子进程去执行这个东西
                if (fork() == 0)
                {
                    execl("/usr/bin/ls", "-l", "-i", "-n", NULL);
                    exit(1);
                }
                waitpid(-1, NULL, 0);
            }
            else if (strcmp(buffer, "exit") == 0)
            {
                printf("server exit ~\n");
                exit(1);
            }
            else if (strcmp(buffer, "run") == 0)
            {
                if (fork() == 0)
                {
                    execl("/usr/bin/sl", "", NULL);
                    exit(1);
                }
                waitpid(-1, NULL, 0);
            }
            else 
            {
                printf("client# : %s\n", buffer);
            }   
        }
        else if (s == 0)
        {
            // fifo close
            printf("client quit ...\n");
            break;
        }
        else 
        {
            // error 
            perror("read fail\n");
        }
    }
    return 0;
}
