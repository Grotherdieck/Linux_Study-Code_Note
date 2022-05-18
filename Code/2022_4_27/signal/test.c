#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    int ret = alarm(30);
    while (1)
    {
        printf("I am a process : ret: %d\n", ret);
        sleep(1);
        int res = alarm(0);// 取消上一个设置的闹钟 获得剩余秒数
        printf("res : %d\n", res);
    }
    return 0;
}

//void handler(int signal)
//{
//    switch(signal)
//    {
//        case 2:
//            printf("hello my friend, I got a signal :%d\n", signal);
//            break;
//        case 3:
//            printf("hello router, I got A signal : %d\n", signal);
//            break;
//        case 9:
//            printf("hello linux, I got A signal : %d\n", signal);
//            break;
//        default:
//            printf("I got signo : %d\n", signal);
//            break;
//    }
//    exit(1);
//   // printf("I got a signal, signal id : %d, pid : %d \n", signal, getpid());
//}
//
//
//int main()
//{
//    // 通过signal函数 把2号动作处理为我们的特定动作
//    // signal(2, handler);
//    //for (int i = 1; i <= 31; ++i)
//    //{
//    //    signal(i, handler);
//    //}
//    while (1)
//    {
//        int a = 1;
//        // a /= 0;
//        //int* p = NULL;
//        //p = (int*)100;
//        //*p = 100;
//        printf("hello world! My pid : %d\n", getpid());
//        sleep(1);
//    }
//    return 0;
//}

//int main()
//{
//    if (fork() == 0)
//    {
//        int a = 1;
//        a /= 0;
//        exit(1);
//    }
//    int status = 0;
//    waitpid(-1, &status, 0);
//    printf("exit code %d\n, exit sig : %d\n, core dump flag = %d\n", (status >> 8)&0xff, status & 0x7f, status >> 7 & 1);
//}
// 

// my kill ./test signo who

// 使用手册
//static void Usage(const char *proc)
//{
//    printf("Usage:\n\t %s signo who\n", proc);
//}
//
//int main(int argc, char* argv[])
//{
//    if (argc != 3) {
//        Usage(argv[0]);
//        return 1;
//    }
//    int signo = atoi(argv[1]);
//    int who = atoi(argv[2]);
//
//    kill(who, signo);
//    return 0;
//}
//


//void handler(int signal)
//{
//    switch(signal)
//    {
//        case 2:
//            printf("hello my friend, I got a signal :%d\n", signal);
//            break;
//        case 3:
//            printf("hello router, I got A signal : %d\n", signal);
//            break;
//        case 9:
//            printf("hello linux, I got A signal : %d\n", signal);
//            break;
//        default:
//            printf("I got signo : %d\n", signal);
//            break;
//    }
//    //exit(1);
//   // printf("I got a signal, signal id : %d, pid : %d \n", signal, getpid());
//}
//int main()
//{
//    for (int i = 1; i < 31; ++i) signal(i, handler);
//    //abort();
//    //for (int i = 1; i < 31; ++i)
//    //{
//    //    if (i != 9) raise(i);
//    //}
//    alarm(6);
//    while (1)
//    {
//        printf("hello world\n");
//        sleep(1);
//    }
//    return 0;
//}
// 

//int count = 0;
//
//void handler(int signo)
//{
//    printf("hello count : %d\n", count);
//    exit(1);
//}
//
//int main()
//{
//    
//    // 测一下1s 对int能增加多少次
//    signal(14, handler);
//    alarm(1);
//    while (1)
//    {
//        // printf("%d\n", count++);
//        ++count;
//    }
//    return 0;
//}
