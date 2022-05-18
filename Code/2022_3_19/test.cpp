#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int id = fork();
    if (id == 0)
    {
        // exce*
        // 返回了就退出
        execl("/usr/bin/ls", "ls", "-a", "-l", "-i", "-n", NULL);
        exit(-1);
    }
    waitpid(id, NULL, 0);
    printf("wait success->%d\n", id);
    return 0;
}



//int main()
//{
//    pid_t id = fork();
//    if (id == 0)
//    {
//        printf("I am a process, pid:%d\n", getpid());
//
//        execl("/usr/bin/ls", "ls", "-a", "-l", NULL);
//
//        printf("hahahahaha");
//        printf("hahahahaha");
//        printf("hahahahaha");
//        printf("hahahahaha");
//        printf("hahahahaha");
//        printf("hahahahaha");
//        printf("hahahahaha");
//
//    }
//    int ret = waitpid(id, NULL, 0);
//    if (ret > 0)
//    {
//        printf("father wait success:%d\n", ret);
//    }
//    else 
//    {
//        printf("father wait fail\n");
//        exit(-1);
//    }
//    return 0;
//}
//
//
//int main()
//{
//    pid_t id = fork();
//    if (id == 0)
//    {
//        // child
//        // int a = 10;
//        // a /= 0;
//        int count = 5;
//        while (count)
//        {
//            printf("child[%d] is running:count = %d\n", getpid(), count);
//            --count;
//            sleep(1);
//        }
//        exit(20);
//    }
//    int status = 0;
//    while (1)
//    {
//
//        pid_t ret = waitpid(id, &status, WNOHANG);
//        if (ret == 0)
//        {
//            // 子进程没有退出，但是waitpid是等待成功的，需要父进程重复进行等待
//            // 就是打电话 子进程还没退出呢
//            printf("do father thing!\n");
//            sleep(1);
//        }
//        else if (ret > 0)
//        {
//            printf("father wait:%d, exitcode = %d, exitsignal = %d\n", ret, (status >> 8) & 0xff, status & 0x7f );
//            // 子进程退出了 并且waitpid也成功了 获取对应结果即可
//            break;
//        }
//        else 
//        {
//            // ret < 0 等待失败
//            perror("waitpid failed\n");
//        }
//    }
// parent
//sleep(10);
//printf("father wait begin\n");
//int status = 0;
//pid_t ret = waitpid(id, &status, 0);
//if (ret > 0)
//{
//    // printf("father wait:%d, exitcode = %d, exitsignal = %d\n", ret, (status >> 8) & 0xff, status & 0x7f );
//    if (WIFEXITED(status))
//    {
//        printf("child exit normal, exit code: %d\n", WEXITSTATUS(status));
//    }
//
//}
//else 
//{
//    printf("father wait failed\n");
//}
//sleep(10);
// 

//int main()
//{
//   // int a = 10;
//   // a /= 0;
//   // for (int i = 0; i < 140; ++i)
//   // {
//   //     printf("%d->%s\n", i, strerror(i));
//   //     if (i == 12)
//   //     {
//   //         exit(i);
//   //     }
//   // }
//   // return 100;
//    printf("hello world!");
//    sleep(4);
//    _exit(12);
//}
