#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
    if (fork() == 0)
    {
        printf("command begin..\n");
        // execl("/usr/bin/ls", "ls", "-l", "-a", "-n", "-i", NULL);
        //char* argv[] = {"ls", "-l","-a", "-i", NULL };
        //execvp("ls", argv);
        //execv("/usr/bin/ls", argv);
        //execlp("ls", "ls", "-a", "-i", NULL);
        //execl("./myexe", "myexe", NULL);
        //char* env[] = {"E1=HAHAHAHAHA", "E2=hahahahahaha", "E3=heheheheheh", "E4=HEHEHEHEHHE"};
        // execle("./myexe", "myexe", NULL, env);
        //char* argv[] = {"./myexe", NULL};
        //execve("./myexe", argv, env);
        execl("/usr/bin/python3", "python", "test.py", NULL);
        printf("command end..\n");
        exit(1);
    }
    waitpid(-1, NULL, 0);
    printf("wait child success\n");
    return 0;
}
