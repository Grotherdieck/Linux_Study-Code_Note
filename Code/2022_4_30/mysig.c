#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

void handler(int signo)
{
    while (1)
    {
        printf("Get a signo :%d\n", signo);
    }
}

int main()
{
    struct sigaction act;
    // 全部初始化为0
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    // act.sa_handler = SIG_IGN;// 忽略
    // act.sa_handler = SIG_DFS;// 默认动作
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);
    sigaction(2, &act, NULL);
    while (1)
    {
        printf("hello myfriend\n");
        sleep(1);
    }
    return 0;
}


//void handler(int signo)
//{
//    printf("%d号信号被递达了，已处理完成!\n");
//}
//
//void show_pending(sigset_t *set)
//{
//    printf("curprocess pending set : ");
//    int i;
//    for (i = 1; i <= 31; ++i)
//    {
//        if (sigismember(set, i))
//        {
//            printf("1");
//        }
//        else printf("0");
//    }
//    printf("\n");
//}
//
//int main()
//{
//    // 虽然sigset_t是一个位图结构 但是不同的OS的操作是不同的
//    // 不能让用户直接修改该变量
//    // 需要使用特定的"系统调用"
//    // set是一个变量，这个变量保存在哪呢？
//    // 肯定是栈区(用户栈)呀
//    // sigset_t set; 
//    sigset_t inset, outset;
//    sigemptyset(&inset), sigemptyset(&outset);
//    // 屏蔽2号信号
//    sigaddset(&inset, 2);
//    // 尝试屏蔽9号信号
//    // sigaddset(&inset, 9);
//    // 添加到位图中 并且获得当前进程老的屏蔽字
//    sigprocmask(SIG_SETMASK, &inset, &outset);
//    signal(2, handler);
//    sigset_t pending;
//    int count = 0;
//    while (1)
//    {
//        // 清空位图
//        sigemptyset(&pending);
//
//        // 获取当前pending位图
//        sigpending(&pending);
//
//        // 打印pending位图
//        show_pending(&pending);
//
//        // printf("hello SCU, mypid : %d\n", getpid());
//        sleep(1);
//        count++;
//        if (count == 10) 
//        {
//            sigprocmask(SIG_SETMASK, &outset, NULL);
//            printf("对2号信号的屏蔽已经恢复\n");
//        }
//    }
//    return 0;
//}
