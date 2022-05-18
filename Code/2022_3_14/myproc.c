#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    return 100;
}

//int main()
//{
//    const char* p1 = "hello world";
//    const char* p2 = "hello world";
//    printf("%p\n", p1);
//    printf("%p\n", p2);
//    return 0;
//}

//int g_val = 1;
//
//int main()
//{
//    if (fork() == 0)
//    {
//        int cnt = 5;
//        while (cnt)
//        {
//            printf("I am child, cnt = %d, g_val = %d, &g_val = %p\n", cnt, g_val, &g_val);
//            cnt--;
//            if (cnt == 3)
//            {
//                g_val = 200;
//            }
//            sleep(1);
//        }
//    }
//    else 
//    {
//        while (1)
//        {
//            printf("I am parent, g_val = %d, &g_val = %p\n", g_val, &g_val);
//            sleep(1);
//        }
//    }
//    return 0;
//}
//
//

//
//int uninit_g;
//int init_g = 100;
//
//
//int main(int argc, char* argv[], char* env[])
//{
//    printf("code addr:%p\n", main);
//    const char* str = "hello";
//    printf("string rdonly addr:%p\n", str);
//    printf("uninit addr:%p\n", &uninit_g);
//    printf("init addr:%p\n", &init_g);
//    char* heap = (char*)malloc(10);
//    char* heap1 = (char*)malloc(10);
//    char* heap2 = (char*)malloc(10);
//    char* heap3 = (char*)malloc(10);
//    printf("heap addr:%p\n", heap);
//    printf("heap addr:%p\n", heap1);
//    printf("heap addr:%p\n", heap2);
//    printf("heap addr:%p\n", heap3);
//    printf("stack addr:%p\n", &str);
//
//    int a = 1;
//    int b = 2;
//    // 验证栈区向低地址增长
//    printf("stack addr:%p\n", &a);
//    printf("stack addr:%p\n", &b);
//    for (int i = 0; argv[i]; ++i)
//    {
//        printf("argv[%d]:%p\n", i, argv[i]);
//    }
//    for (int i = 0; env[i]; ++i)
//    {
//        printf("env[%d]:%p\n", i, env[i]);
//    }
//    return 0;
//}








//int main()
//{
//    int cnt = 5;
//    while (cnt--)
//    {
//        // printf("I am a process, pid : %d, ppid : %d\n", getpid(), getppid());
//        printf("I am a cmd -> process\n");
//        sleep(1);
//
//    }
//
//    return 0;
//}
//int main(int argc, char* argv[])
//{
//    //  for (int i = 0; i < argc; ++i)
//    //  {
//    //      printf("argv[%d]->%s\n", i, argv[i]);
//    //  }
//    if (argc != 1)
//    {
//        if (strcmp(argv[1], "-a") == 0)
//        {
//            printf("hello myfriend\n");
//        }
//        else if (strcmp(argv[1], "-l") == 0)
//        {
//            printf("hello Router\n");
//        }
//    }
//    else 
//    {
//        printf("Hello world\n");
//    }
//    return 0;
//}

//int main(int argc, char* argv[], char* env[])
//{
//    for (int i = 0; env[i]; ++i)
//    {
//        printf("env[%d]->%s\n", i, env[i]);
//    }
//    return 0;
//}


//int main()
//{
//    extern char** environ;
//    for (int i = 0; environ[i]; ++i)
//    {
//        printf("environ[%d]->%s\n", i, environ[i]);
//    }
//    return 0;
//}

//int main()
//{
//    printf("PATH=%s\n", getenv("PATH"));
//    printf("SHELL=%s\n", getenv("SHELL"));
//    printf("HOME=%s\n", getenv("HOME"));
//    return 0;
//}

//int main()
//{
//    printf("Router_string=%s\n", getenv("Router_string"));
//    return 0;
//}


