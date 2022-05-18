#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main()
{
#define NUM 100
    char bar[NUM + 1];
    const char* label = "|/-\\";
    memset(bar, 0, sizeof(bar));
    int i = 0;
    while (i <= 100)
    {
        printf("[\033[1;40;37m%-100s\033[0m][\033[1;44;32m%-3d%%\033[0m]\033[1;43;34m%c\033[0m\r", bar, i, label[i % 4]);
        fflush(stdout);
        bar[i] = '#';
        i++;
        usleep(50000);
    }
    printf("\n");
    return 0;
}
