#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    //FILE* fp = fopen("./bite", "w");
    //if (fp == NULL)
    //{
    //    perror("fopen fail\n");
    //    exit(1);
    //}
    //fprintf(fp, "\nlinux so easy!\n");
    //fclose(fp);
    //fp = fopen("./bite", "r");
    //char _buf[128];
    //while (fgets(_buf, sizeof(_buf), fp))
    //{
    //    printf("%s\n", _buf);
    //}
    //fclose(fp);
    //return 0;
    int fd = open("./bite", O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open file fail\n");
        exit(1);
    }
    char _buf[128] = "i like Linux!";
    write(fd, _buf, strlen(_buf));
    close(fd);
    int rfd = open("./bite", O_RDONLY);
    char buffer[128];
    int s = read(rfd, buffer, sizeof(buffer)); 
    buffer[s] = '\0';
    printf("%s\n", buffer);
    close(rfd);
    return 0;
}
