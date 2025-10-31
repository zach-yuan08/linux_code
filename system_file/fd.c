#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
  close(1);
  int fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  printf("fd = %d", fd);
  
  return 0;
}

//int main()
//{
//  int fd1 = open("log1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
//  int fd2 = open("log2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
//  int fd3 = open("log3.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
//  int fd4 = open("log4.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
//  printf("fd1:%d\n", fd1);
//  printf("fd2:%d\n", fd2);
//  printf("fd3:%d\n", fd3);
//  printf("fd4:%d\n", fd4);
//
//  close(fd1);
//  close(fd2);
//  close(fd3);
//  close(fd4);
//  return 0;
//}
