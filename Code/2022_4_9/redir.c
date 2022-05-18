#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 系统调用
    const char* msg = "Good morning, 标准输出\n";
    write(1, msg, strlen(msg));
    //const char* msg1 = "Good morning, 标准错误\n";
    //write(2, msg1, strlen(msg1));
    // C语言接口
    printf("hello printf\n");// FILE* ->FILE->buffer->fd->内核中 buffer刷新策略和文件类型有关
    fprintf(stdout, "hello fprintf\n");
    fputs("hello fputs\n", stdout);
    //close(1);
    fflush(stdout);
    fork();
    return 0;
}
