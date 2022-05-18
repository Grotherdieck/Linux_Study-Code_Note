#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* thread_run(void* args)
{
    while (1)
    {
        printf("thread1 id : 0x%x", pthread_self());
        sleep(1);
    }
    return args;
}



int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_run, (void*)"thread1");
    while (1)
    {
        printf("main thread id : 0x%x \n", pthread_self());
        sleep(1);
    }
    return 0;
}























//pthread_t g_tid_t;
//
//
//void* thread_run(void* args)
//{
//    pthread_detach(pthread_self());
//    int num = *(int*)args;
//    while (1)
//    {
//        printf("我是新线程%d，我的id是%u\n", num, pthread_self());
//        //sleep(10);
//        //if (num == 3)
//        //{
//        //    printf("thread number : %d quit \n", num);
//        //    int *p = NULL;
//        //    *p = 100;
//        //}
//        // 新线程退出主线程
//        //pthread_cancel(g_tid_t);
//        sleep(15);
//        pthread_exit((void*)100);
//        break;
//    }
//    return (void*)111;
//}
//
//#define NUM 1
//
//int main()
//{
//    g_tid_t = pthread_self();
//    // 创建多个线程
//    pthread_t tid[NUM];
//    for (int i = 0; i < NUM; ++i)
//    {
//        pthread_create(&tid[i], NULL, thread_run, (void*)&i);
//        sleep(1);
//    }
//    
//    printf("wait subthread...\n");
//    sleep(15);
//    pthread_cancel(tid[0]);
//
//    // 等待tid[0]线程退出
//    void* status = NULL;
//    int ret = 0;
//    for (int i = 0; i < NUM; ++i)
//    {
//        ret = pthread_join(tid[i], &status);
//    }
//    printf("status : %d, ret : %d\n", (int)status, ret);
//    //while (1)
//    //{
//    //    printf("我是主线程，我的线程id为%u\n", pthread_self());
//    //    for (int i = 0; i < 5; ++i)
//    //    {
//    //        printf("#################begin######################\n");
//    //        printf("我创建的线程[%d]id为%u\n", i, tid[i]);
//    //        printf("#################end########################\n");
//    //    }
//    //    sleep(1);
//    //}
//    return 0;
//}
