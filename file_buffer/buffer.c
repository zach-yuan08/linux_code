#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{

  printf("hello printf\n");
  fprintf(stdout, "hello fprintf\n");
  const char* s = "hello fwrite\n";
  fwrite(s, strlen(s), 1, stdout);

  const char* ss = "hello write\n";
  write(1, ss, strlen(ss));

  fork();

  return 0;
}



//int main()
//{ 
//  close(1);
//
//  int fd = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0666);
//
//  printf("fd: %d \n", fd);
//  printf("hello linux\n");
//  printf("hello linux\n");
//  printf("hello linux\n");
//  printf("hello linux\n");
//
//  const char* msg = "hello write\n";
//  write(fd , msg, strlen(msg));
//  
//   close(fd);
//  return 0;
//}
