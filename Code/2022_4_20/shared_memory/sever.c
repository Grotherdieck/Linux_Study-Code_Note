#include "comm.h"

int main()
{
    // 获取key值
    key_t key = ftok(PATH_NAME, PRO_ID);
    if (key < 0) {
        perror("ftok fail\n");
        exit(1);
    }
    // 创建共享内存
    // int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL);
    int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0) {
        perror("shmget error\n");
        exit(2);
    }
    printf("key : 0x%x shmid : %d\n", key, shmid);

    sleep(10);
    shmctl(shmid, IPC_RMID, NULL);

    printf("key : 0x%x shmid : %d -> delete success\n", key, shmid);
    sleep(10);
    return 0;
}
