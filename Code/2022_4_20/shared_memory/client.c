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
   int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL);
   if (shmid < 0) {
       perror("shmget error\n");
       exit(2);
   }
   printf("key : %d shmid : %d\n", key, shmid);


    return 0;
}
