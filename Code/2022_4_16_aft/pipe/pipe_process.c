#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


int main()
{
    int pipefd[2] = {0};
    if (pipe(pipefd) != 0)
    {
        // 创建失败
        perror("pipe fail\n");
        return 1;
    }
    // 创建成功
    // pipefd[0]是读取端 pipefd[1]是写入端
    printf("%d\n", pipefd[0]);
    printf("%d\n", pipefd[1]);
    // 我们想让父进程读取 子进程写入
    if (fork() == 0)
    {
        // 子进程
        close(pipefd[0]);
        const char* msg = "hello Router\n";
        int count = 1;
        // 写端不停的写
        while (1)
        {
            //write(pipefd[1], msg, strlen(msg));
            write(pipefd[1], "a", 1);
            //printf("count = %d\n", count);
            count++;
            // 验证写端关闭 读端退出
            // break;
            // sleep(1); 子进程不睡了
        }
        close(pipefd[1]);
        exit(0);
    }
    else 
    {
        // 父进程
        close(pipefd[1]);
        // 父亲读
        while (1)
        {
            char _buf[128] = {0};
            // 父进程睡一下
            sleep(10);
            // 父进程读一个
            char c[1024 * 2 + 1] = {0};
            ssize_t s = read(pipefd[0], c, sizeof(c) - 1);
            if (s > 0)
            {
                c[s] = 0;
                printf("father take : %c\n", c[0]);
            }
            else if (s == 0) 
            {
                printf("read quit...\n");
                break;
            }
            else break;
            // 读端读一条就退出
            break;
            // 改为父进程不读取
            // int s = read(pipefd[0], _buf, sizeof(_buf) - 1);
            // if (s == 0) 
            // {
            //     printf("child quit...\n");
            //     break;
            // }
            // else if (s > 0)
            // {
            //     // 如果此时read返回值为0 在管道中意味着子进程关闭了文件描述符了
            //     _buf[s] = '\0';
            //     printf("child say:%s\n", _buf);
            // }
            // else
            // { 
            //     printf("read error\n");
            //     break;
            // }
        }
        close(pipefd[0]);
        int status = 0;
        waitpid(-1, &status, 0);
        // status中查看子进程退出
        printf("exit code : %d\n", (status >> 8) & 0xFF);
        printf("exit signal : %d\n", status & 0x7f);
    }
    return 0;
}
