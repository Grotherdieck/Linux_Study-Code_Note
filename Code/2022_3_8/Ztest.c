#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h> 
int main() 
{  
    int ret = fork();
    if (ret == 0)
    {
        while (1)
        {
            printf("I am child running\n");
            sleep(2);
        }
    }
    else 
    {
        printf("Father died soon...\n");
        sleep(10);
        exit(1);
    }
    return 0;
}
