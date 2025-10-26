#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  printf("我的程序要运行了：\n");
  execl("/usr/bin/ls", "ls", "-l", "-a", NULL);
  printf("我的程序运行完毕。\n");
  return 0;
}






















//int main()
//{
//  pid_t id = fork();
//  if(id == 0)
//  {
//    //子进程
//    int cnt = 3;
//    while(cnt)
//    {
//      printf("我是一个子进程, pid : %d, ppid : %d\n", getpid(), getppid());
//      sleep(1);
//      cnt--;
//    }
//    exit(1);
//  }
//  int status = 0;
//  pid_t rid = waitpid(id, &status, 0);
//  if(rid > 0)
//  {
//    printf("wait success, rid: %d, status: %d\n", rid, status); // rid
//  }
//  else
//  {
//    printf("wait failed: %d: %s\n", errno, strerror(errno));
//  }
//  return 0;
//}
//int main()
//{
//  pid_t id = fork();
//  if(id == 0)
//  {
//    // 子进程
//    int cnt = 5;
//    while(cnt--)
//    {
//      printf("子进程,pid:%d, ppid:%d\n", getpid(), getppid());
//      sleep(1);
//    }
//    exit(0);
//  }
//
//  sleep(10);
//  pid_t rid = wait(NULL);
//  sleep(10);
//  return 0;
//}
//
//
//int main()
//{
//  printf("hello");
//  _exit(0);
//}
//
//void fun()
//{
//	printf("fun begin\n");
//	exit(40);
//	printf("fun end\n");
//}
//
//int main()
//{
//	fun();
//	printf("main\n");
//	return 0;
//}
//

//int main()
//{
//  int a = 10;
//  a /= 0;
//  return 0;
//}
////int main()
//{
//  for(int i = 0; i < 200; i++)
//  {
//    printf("%d->%s\n", i, strerror(i));
//  }
//
//  return 0;
//}
