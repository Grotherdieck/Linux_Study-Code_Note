#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  //  while (1)
  //  {
  //      printf("hello world!:pid:%d ppid%d\n", getpid(), getppid());
  //      sleep(1);
  //  }
    int ret = fork();
  //  if (ret > 0)
  //  {
  //      printf("You hear me.\n");
  //  }
  //  else 
  //  {
  //      printf("You see me.\n");
  //  }
  //  sleep(1);
    while (1) 
    {
        printf("I am a process , my pid:%d, my ppid:%d\n", getpid(), getppid());
        sleep(1);
    }
    return 0;
}
