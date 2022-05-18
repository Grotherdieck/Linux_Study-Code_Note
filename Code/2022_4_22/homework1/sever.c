#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH "./myfifo"

int main()
{
    umask(0);
    if (mkfifo(PATH, 0666) < 0)
    {
        perror("mkfifo fail");
        return 1;
    }
    // 像普通文件一样打开并读
    int fd = open(PATH, O_RDONLY);
    if (fd < 0)
    {
        perror("open fail");
        return 1;
    }
    char buffer[64] = {0};
    ssize_t s = read(fd, buffer, sizeof(buffer) - 1);
    if (s < 0) {
        perror("read fail");
        return 1;
    }
    else if (s > 0)
    {
        buffer[s] = 0;
        printf("%s\n", buffer);
    }
    close(fd);
    return 0;
}

