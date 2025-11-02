#include "mystdio.h"
#include <string.h>
int main()
{
  my_file* filep = my_fopen("./log.txt", "a");
  if(!filep)
  {
    printf("fopen error\n");
    return 1;
  }

  const char* msg = "hello myfile\n";
  my_fwrite(filep, msg, strlen(msg));
  my_fclose(filep);
  return 0;
}
