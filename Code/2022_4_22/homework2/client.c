#include "comm.h"
#include <string.h>
int main()
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok fail");
        return 1;
    }
    int shmid = shmget(key, SIZE, IPC_CREAT);
    char* author = (char*)shmat(shmid, NULL, 0);
    char str[64] = "I am a process";
    strcpy(author, str);
    shmdt(author);
    return 0;
}
