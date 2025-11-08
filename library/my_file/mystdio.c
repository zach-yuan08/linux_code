#include "mystdio.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


my_file* buy_file(int fd, int flag)
{
  my_file* f = (my_file*)malloc(sizeof(my_file));
  if(f == NULL) return NULL;
  f->bufferlen = 0;
  f->fileno = fd;
  f->flag = flag;
  memset(f->outbuffer, 0, sizeof(f->outbuffer));
  return f;
}

my_file* my_fopen(const char* path, const char* mode)
{
  int fd = -1;
  int flag = 0;
  if(strcmp(mode, "w") == 0)
  {
    flag = O_CREAT | O_WRONLY | O_TRUNC;
    fd = open(path, flag, 0666);
  }
  else if(strcmp(mode, "a") == 0)
  {
    flag = O_CREAT | O_WRONLY | O_APPEND;
    fd = open(path, flag, 0666);
  }
  else if(strcmp(mode, "r") == 0)
  {
    flag = O_RDWR;
    fd = open(path, flag);
  }
  return buy_file(fd, flag);
}

void my_fclose(my_file* file)
{
  if(file->fileno < 0) return;

  my_fflush(file);
  close(file->fileno);
  free(file);
}

int my_fwrite(my_file* file, const void* str, int len)
{ 
  memcpy(file->outbuffer + file->bufferlen, str, len);
  file->bufferlen += len;
  if((file->flush_method & LINE_FLUSH) && file->outbuffer[file->bufferlen - 1] == "\n")
    my_fflush(file);
  return 0;
}

void my_fflush(my_file* file)
{
  if(file->bufferlen <= 0) 
    return;
  int n = write(file->fileno, file->outbuffer, file->bufferlen);
  (void)n;
  fsync(file->fileno);
  file->bufferlen = 0;
}


