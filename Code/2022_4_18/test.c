#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int pipefd[2] = {0};
    if (pipe(pipefd) == 0)
    {
        if (fork() == 0)
        {
            // 子进程
            close(pipefd[1]);
            char _buf[64] = {0};
            ssize_t s = read(pipefd[0], _buf, sizeof(_buf) - 1);
            _buf[s] = 0;
            printf("receive message from father:%s\n", _buf);
        }
        else 
        {
            // 父进程写
            close(pipefd[0]);
            const char* str = "I am father.";
            write(pipefd[1], str, strlen(str));
        }

    }
    else 
    {
        perror("pipe fail\n");
        exit(1);
    }
    return 0;
}
