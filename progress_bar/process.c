#include "process.h"
#include <string.h>
#include <unistd.h>

#define SIZE 101
#define STYLE '#'
void process()
{
  int rate = 0;
  char buffer[SIZE];
  memset(buffer, 0, sizeof(buffer));
  const char* lable = "|/-\\";
  int len = strlen(lable);
  while(rate <= 100)
  {
    printf("[%-100s][%d%%][%c]\r", buffer, rate, lable[rate % len]);
    fflush(stdout);
    buffer[rate++] = STYLE;
    usleep(100000);
  }

  printf("\n");
}

void flush_process(double total, double current)
{
   char buffer[SIZE];
  memset(buffer, 0, sizeof(buffer));
  const char *lable="|/-\\";
  int len = strlen(lable);
  static int cnt = 0;
  // 不需要⾃⼰循环,填充#
  int num = (int)(current*100/total); // 11.0 / 1000
  int i = 0;
  for(; i < num; i++)
  {
    buffer[i] = STYLE;
  }
  double rate = current/total;
  cnt %= len;
  printf("[%-100s][%.1f%%][%c]\r", buffer, rate*100, lable[cnt]);
  cnt++;
  fflush(stdout);
}
