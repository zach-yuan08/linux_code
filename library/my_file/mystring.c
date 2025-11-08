#include "mystring.h"

int my_strlen(char *s)
{
  char* start = s;
  while(*s)
  {
    s++;
  }
  return s - start; 
}
