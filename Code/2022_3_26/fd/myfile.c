#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd = open("./log.txt", O_WRONLY | O_CREAT,0644);
    int fd1 = open("./log1.txt", O_WRONLY | O_CREAT,0644);
    int fd2 = open("./log2.txt", O_WRONLY | O_CREAT,0644);
    int fd3 = open("./log3.txt", O_WRONLY | O_CREAT,0644);
    int fd4 = open("./log4.txt", O_WRONLY | O_CREAT,0644);
    int fd5 = open("./log5.txt", O_WRONLY | O_CREAT,0644);
    if (fd < 0)
    {
        perror("open error\n");
        return 1;
    }
    printf("%d\n", fd);
    printf("%d\n", fd1);
    printf("%d\n", fd2);
    printf("%d\n", fd3);
    printf("%d\n", fd4);
    printf("%d\n", fd5);
    close(fd);
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);

    //const char* msg = "You see me\n";
    //fputs(msg, stdout);
    //perror(msg);
    //FILE* fp = fopen("./log.txt", "a");
    //if (fp == NULL)
    //{
    //    perror("fopen:");
    //    return 1;
    //}
    //int cnt = 10;
    //const char* msg = "hello myfriend\n";
    //while (cnt--)
    //{
    //    fputs(msg, fp);
    //}
    ////char buffer[64];
    ////while (fgets(buffer, sizeof(buffer), fp))
    ////{
    ////    printf("%s", buffer);
    ////}

    ////if (!feof(fp))
    ////{
    ////    printf("fgets quits not normal\n");
    ////}
    //fclose(fp);
    return 0;
}
