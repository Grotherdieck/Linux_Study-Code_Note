#include "comm.h"

int main()
{
    // 获取key值
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok fail");
        return 1;
    }
    // 创建共享内存 shmget
    int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0) {
        perror("shmget fail");
        return 2;
    }
    // 挂接共享内存 shmat
    char* pshm = (char*)shmat(shmid, NULL, 0);
    sleep(10);
    // 写内容
    printf("%s\n", pshm);
    // 取消挂接
    shmdt(pshm);
    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
