#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
  umask(0);
  int fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  if(fd < 0)
  {
    perror("open");
    return 1;
  }
  
  printf("%d\n", fd);

  const char* msg = "hello linux\n";
  int cnt = 5;
  while(cnt--)
  {
    write(fd, msg, strlen(msg));
  }
  close(fd);
  return 0;
}
