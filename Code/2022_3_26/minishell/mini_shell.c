#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>

#define NUM 128
// 所谓的一个命令行解释器 不就是先打印一个提示符然后等待你输入字符串嘛
#define CMDNUM 64
int main()
{
    char command[NUM];
    // 先获得用户id号
    uid_t userid = getuid();
    while (1)
    {
        // 获取用户名字
        struct passwd* pwd = getpwuid(userid);
        char* argv[CMDNUM] = {NULL};
        // 先把字符串清空
        command[0] = 0;
        // 获取工作路径
        char buf[NUM] = {0};
        getcwd(buf, sizeof(buf));
        // 1.打印提示符
        printf("[%s@myhostname %s]# ", pwd->pw_name, buf);
        // 把字符串刷新出来
        fflush(stdout);
        // 2.获取命令字符串
        // 直接获取一行的字符串
        fgets(command, NUM, stdin);
        // 把\0清掉
        command[strlen(command) - 1] = '\0';
        //printf("echo %s\n", command);
        // 3.分割命令字符串 解析成char* argv[]这样
        // 利用strtok()
        const char* sep = " ";
        argv[0] = strtok(command, sep);
        int i = 1;
        // 如果还要从原来的串往后分割 传NULL
        while (1)
        {
            argv[i] = strtok(NULL, sep);
            if (argv[i] == NULL) break;
            ++i;
        }
        // 测试一下分割结果
        //for (int j = 0; argv[j]; ++j)
        //{
        //    printf("argv[%d]->%s\n", j, argv[j]);
        //}
        // 4. 检测命令是否是需要shell本身执行的内建命令
        if (strcmp(argv[0], "cd") == 0)
        {
            if (argv[1] != NULL)
                chdir(argv[1]);
            continue;
        }


        // 5.让子进程去执行程序
        if (fork() == 0)
        {
            execvp(argv[0], argv);
            // 如果子进程返回了 说明出错了
            exit(1);
        }
        // 父进程等待
        int status = 0;
        waitpid(-1, &status, 0);
        printf("exit signal : %d exit code : %d\n", status & 0x7f, (status >> 8) & 0xff);
    }
}
