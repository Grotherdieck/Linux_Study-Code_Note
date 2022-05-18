#include <stdio.h>
#include <unistd.h>

int main()
{
    extern char** environ;
    for (int i = 0; environ[i]; ++i)
    {
        printf("%d:%s\n", i, environ[i]);
    }
    printf("myexe finish...\n");
    return 0;
}
