#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <cstdio>
#include <string.h>

//int main()
//{
//    sigset_t myset;
//    sigaddset(&myset, 2), sigaddset(&myset, 40);
//    sigprocmask(SIG_SETMASK, &myset, NULL);
//    while (1)
//    {
//        printf("Hello my friend Router, my pid : %d\n", getpid());
//        sleep(1);
//    }
//    return 0;
// 
//}


//void sigcb(int signo)
//{
//    printf("I got a signal : %d\n", signo);
//}
//
//int main()
//{
//    struct sigaction act;
//    memset(&act, 0, sizeof(act));
//    act.sa_handler = sigcb;
//    sigaction(2, &act, NULL);
//    while (1)
//    {
//        printf("hello my friend\n");
//        sleep(1);
//    }
//    return 0;
//}
// 

void sigcb(int signo)
{
    printf("I got a signal : %d\n", signo);
}

int main()
{
    signal(2, sigcb);
    while (1)
    {
        printf("hello my friend JLW, my pid : %d\n", getpid());
        sleep(1);
    }
    return 0;
}


