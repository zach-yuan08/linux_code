#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
  int ret = fork();
  if(ret < 0)
  {
    perror("fork");
    return 1;
  }
  else if(ret == 0)
  { //child
    printf("I am child : %d!, ret: %d\n", getpid(), ret);
  }
  else
  { //father
    printf("I am father : %d!, ret: %d\n", getpid(), ret);
  }
  sleep(1);
  return 0;
}
