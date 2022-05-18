#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main()
{
    int id = fork();
    if (id == 0)
    {
        int cnt = 10;
        while (cnt--)
        {
            printf("I am child process, my pid:%d\n", getpid());
            sleep(1);
        }
        exit(20);
    }
    // father
    // wait 
    int status = 0;
    // int ret = wait(&status);
    // if (ret == id)
    // {
    //     printf("father wait success!\n");
    //     printf("exitcode : %d, exitsignal : %d, coredump : %d\n", (status >> 8) & 0xff, status & 0x7f, status >> 7 & 1);
    // }
    while (1)
    {
        int ret = waitpid(id, &status, WNOHANG);
        if (ret == 0)
        {
            printf("father do other thing, NOHANG wait\n");
            sleep(1);
        }
        else if (ret > 0)
        {
            //printf("father wait success!\n");
            //printf("exitcode : %d, exitsignal : %d, coredump : %d\n", (status >> 8) & 0xff, status & 0x7f, status >> 7 & 1);
            if (WIFEXITED(status))
            {
                printf("Normal exited, exit code : %d\n", WEXITSTATUS(status));
            }
            else 
            {
                printf("abnormal exit, exit signal : %d, coredump : %d\n", status & 0x7f, status >> 7 & 1);
            }
            break;
        }
        else 
        {
            printf("father wait failed\n");
        }
    }
    return 0;
}
