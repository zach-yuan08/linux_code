#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
  while(1)
  {
    printf("我是一个进程，我的pid：%d, 我的父进程pid：%d\n", getpid(), getppid());
    sleep(1);
  }
  return 0;
}
