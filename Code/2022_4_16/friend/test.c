#include "add.h"
#include "sub.h"
#include <math.h>

int main()
{
    int x = 3;
    int y = 4;
    int r1 = add(x, y);
    int r2 = sub(x, y);
    printf("%d %d\n", r1, r2);
    printf("%f\n", pow(2, 3));
    return 0;
}
