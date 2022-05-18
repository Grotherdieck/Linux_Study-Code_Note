#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    close(1);
    int fd = open("./logx.txt", O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open file fail\n");
        return 1;
    }
    printf("%d\n", fd);
    printf("printf sth...\n");
    fprintf(stdout, "fprintf sth...\n");
    fputs("hello router!\n", stdout);
    close(fd);
    return 0;
    //int fd = open("./log.txt", O_WRONLY | O_TRUNC);// 清空本来的文件
    //if (fd < 0)
    //{
    //    perror("open fail\n");
    //    return 0;
    //}
    //dup2(fd, 1);
    //
    //printf("hello dup\n");
    //fprintf(stdout,"hello dup1\n");
    //fputs("hello dup2\n", stdout);
    //close(fd);
    //printf("%d\n", stdin->_fileno);
    //printf("%d\n", stdout->_fileno);
    //printf("%d\n", stderr->_fileno);
    ////int fd = open("./log.txt", O_RDONLY, 0644);
    //if (fd < 0)
    //{
    //    perror("open file\n");
    //    exit(1);
    //}
    //char _buf[1024];
    //int s = read(fd, _buf, sizeof(_buf));
    //if (s > 0)
    //{
    //    _buf[s] = 0;
    //    printf("%s\n", _buf);
    //}
    //close(fd);
    //return 0;
    //close(1);
    //int fd = open("./log.txt", O_WRONLY | O_CREAT, 0644);
    //fprintf(stdout,"%d\n", fd);
    //fprintf(stdout,"%d\n", fd);
    //fprintf(stdout,"%d\n", fd);
    //fprintf(stdout,"%d\n", fd);
    //fprintf(stdout,"%d\n", fd);
    //fprintf(stdout,"%d\n", fd);
    //char buffer[64];
    //ssize_t s = read(0, buffer, sizeof(buffer));
    //buffer[s - 1] = 0;
    //printf("%s\n", buffer);
    //const char* msg = "hello world\n";
    //write(1, msg, strlen(msg));
    //write(2, msg, strlen(msg));
    //int fileid = open("./log.txt", O_CREAT | O_WRONLY, 0644);
    //int fileid1 = open("./log1.txt", O_CREAT | O_WRONLY, 0644);
    //int fileid2 = open("./log2.txt", O_CREAT | O_WRONLY, 0644);
    //int fileid3 = open("./log3.txt", O_CREAT | O_WRONLY, 0644);
    //int fileid4 = open("./log4.txt", O_CREAT | O_WRONLY, 0644);
   //// if (fileid < 0)
   //// {
   ////     perror("open file failed\n");
   ////     exit(1);
   //// }
   //// int cnt = 5;
   //// const char* msg = "hello my friend\n";
   //// while (cnt--)
   //// {
   ////     write(fileid, msg, strlen(msg));
   ////     // 我们写入字符串时 不需要把\0写入
   ////     // 因为\0是C/C++的标准 系统只关心字符串的内容
   ////
   //// }
    //printf("%d %d %d %d %d\n", fileid, fileid1, fileid2, fileid3, fileid4);
    //close(fileid);
    //close(fileid1);
    //close(fileid2);
    //close(fileid3);
    //close(fileid4);
    return 0;
}
