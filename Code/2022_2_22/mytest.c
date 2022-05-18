#include <stdio.h>

int sumtotop(int x)
{
    int ret = 0;
    for (int i = 1; i <= x; ++i)
    {
        ret += i;
    }
    return ret;
}

int main()
{
    int top = 100;
    int res = sumtotop(top);
    printf("%d\n", res);
    return 0;
}
