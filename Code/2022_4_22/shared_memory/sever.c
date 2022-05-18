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
    // 创建全新的共享内存 权限是666
    int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0) {
        perror("shmget error\n");
        exit(2);
    }
    printf("key : 0x%x shmid : %d\n", key, shmid);

    // sleep(10);
    // 挂接到这个共享内存上
    char* shmp = (char*)shmat(shmid, NULL, 0);
    printf("sever process attaches success\n");
    // sleep(15);

    // 在这里就可以进行IPC了

    while (1)
    {
        sleep(1);

        printf("%s\n", shmp);// sever认为共享内存中就是一个字符串
    }

    // 去挂接
    shmdt(shmp);
    // sleep(5);
    printf("sever process detaches success\n");

    shmctl(shmid, IPC_RMID, NULL);

    printf("key : 0x%x shmid : %d -> delete success\n", key, shmid);
    // sleep(10);
    return 0;
}
