#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fd = open("./myfifo", O_WRONLY);
    char buffer[64] = "I am a process";
    write(fd, buffer, strlen(buffer));
    return 0;
}
