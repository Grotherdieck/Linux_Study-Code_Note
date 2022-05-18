#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void Getchild(int signo)
{
    // waitpid();
    printf("I got a signal from my child : %d, my pid : %d\n", signo, getpid());
}


int main()
{
    //signal(17, Getchild);
    signal(17, SIG_IGN);// 验证SIG_IGN会使子进程直接退出 不形成僵尸
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程
        int cnt = 5;
        while (cnt--)
        {
            printf("I am child : %d\n", getpid());
            sleep(1);
        }
        exit(1);
    }
    // 父进程死循环
    while (1);
    return 0;
}
