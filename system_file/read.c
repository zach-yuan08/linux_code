#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
  umask(0);
  int fd = open("log.txt", O_RDONLY); 
  if(fd < 0)
  {
    perror("open");
    return 1;
  }
  
  printf("%d\n", fd);
  
  while(1)
  {
    char buffer[64];
    int n = read(fd, buffer,sizeof(buffer) - 1);
    if(n > 0)
    {
      buffer[n] = 0;
      printf("%s", buffer);
    }
    else if(n == 0)
      break;
  }
  close(fd);
  return 0;
}
