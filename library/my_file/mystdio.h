#pragma once 
#include <stdio.h>

#define MAX 1024

#define NONE_FLUSH (1<<0)
#define LINE_FLUSH (1<<1) 
#define FULL_FLUSH (1<<2)
typedef struct my_file{
  int fileno;
  int flag;
  char* outbuffer[MAX]; // 缓冲区
  int bufferlen;
  int flush_method;
}my_file;

my_file* my_fopen(const char* path, const char* mode);

void my_fclose(my_file*);

int my_fwrite(my_file*, const void* str, int len);

void my_fflush(my_file*);
