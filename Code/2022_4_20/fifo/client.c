#include "comm.h"
#include <string.h>
// 作为客户端 不需要创建管道文件
// 我只要获取即可
int main()
{
    int fd = open(MY_FIFO, O_WRONLY);
    if (fd < 0)
    {
        perror("open fail\n");
        exit(1);
    }
    // 业务逻辑
    while (1)
    {
        printf("请输入:#");
        fflush(stdout);
        char buffer[64] = {0};
        ssize_t s = read(0, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            // 把空格那位置弄掉
            buffer[s - 1] = 0;
            write(fd, buffer, strlen(buffer));

        }
    }
    return 0;
}
