#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* zjjrun(void* args)
{
    const char* id = (const char*)args;
    while (1)
    {
        printf("张君杰无敌\n");
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, zjjrun, (void*)"thread1");
    while (1)
    {
        printf("卢涵宇是傻逼.\n");
        sleep(1);
    }
    return 0;
}
