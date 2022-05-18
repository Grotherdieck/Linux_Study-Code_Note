#include <stdio.h>
#include <unistd.h>
int main()
{
   // printf("weilcome to the new age!\n");
   // printf("weilcome to the new age!");
   // fflush(stdout);
   // sleep(5);
   
    int count = 10;
    while (count)
    {
        printf("%2d\r", count);
        fflush(stdout);
        sleep(5);
        count--;
    }
    return 0;
}
