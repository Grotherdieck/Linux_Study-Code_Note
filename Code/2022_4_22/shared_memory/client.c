#include "comm.h"


int main()
{
    // 获取key值
    key_t key = ftok(PATH_NAME, PRO_ID);
    if (key < 0) {
        perror("ftok fail");
        exit(1);
    } 
    // 获取shmid client不用新建共享内存 client只需要获取即可
    int shmid = shmget(key, SIZE, IPC_CREAT);

    if (shmid < 0) {
        perror("shmget fail");
        return 2;
    }
    
    // 挂接
    char* shmp = (char*)shmat(shmid, NULL, 0);
    // sleep(5);
    printf("client process attaches success\n");

    // 通信区域
    
    char c = 'A';
    while (c <= 'Z')
    {
        shmp[c - 'A'] = c;
        c++;
        // 它后一个位置置成0 这样字符串不断变长
        shmp[c - 'A'] = 0;
        sleep(2);
    }

    // 去挂接
    shmdt(shmp);
    // sleep(5);
    printf("client process detaches success\n");



    return 0;
}
