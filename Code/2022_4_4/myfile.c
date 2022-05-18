#include <sys/types.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    close(1);
    int fd = open("./log.txt", O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open fail\n");
        exit(1);
    }
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    printf("hello buffer\n");
    fflush(stdout);
    close(fd);
    return 0;
    //int fd = open("./log.txt", O_RDONLY);
    //if (fd < 0)
    //{
    //    perror("open fail\n");
    //    exit(1);
    //}
    //dup2(fd, 0);
    //char _buf[128];
    //scanf("%s", _buf);
    //printf("%s\n", _buf);
    //return 0;
    //close(0);
    //int fd = open("./log.txt", O_RDONLY);
    //char _buf[1024];
    //printf("%d\n", fd);
    //while (fgets(_buf, sizeof(_buf) - 1,stdin))
    //{
    //    printf("%s\n", _buf);
    //}
    //close(fd);
    //return 0;
    //close(1);
    //int fd = open("./log.txt", O_CREAT | O_APPEND | O_WRONLY, 0666);
    //if (fd < 0)
    //{
    //    perror("open fail\n");
    //    return 1;
    //}
    ////dup2(fd, 1);
    //printf("hello world\n");
    //fprintf(stdout, "hey man\n");
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fputs("impossible\n", stdout);
    //fflush(stdout);
    //close(fd);
    return 0;
}
