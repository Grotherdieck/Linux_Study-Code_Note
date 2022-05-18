#include <stdio.h>
#include <unistd.h>

int main()
{
    int ret = fork();
    if (ret == 0)
    {
        while (1)
        {
            printf("I am child, my pid: %d, my parent:%d\n", getpid(), getppid());
            sleep(1);
        
        }
    }
    else if (ret > 0)
    {
        while (1)
        {
            printf("I am parent, my pid: %d, my parent:%d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else 
    {
        printf("fork failed\n");
    }
    sleep(1);
    return 0;
}
