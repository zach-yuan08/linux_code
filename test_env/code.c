#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[], char*env[])
{
  (void)argc;
  (void)argv;
  (void)env;

  const char* who = getenv("USER");
  if(who == NULL)
    return 0;
  if(strcmp(who, "yjc") == 0)
    printf("ok!\n");
  else 
    printf("no!\n");
  return 0;
}

//int main(int argc, char* argv[], char*env[])
//{
//  (void)argc;
//  (void)argv;
//  (void)env;
//
//  char *value = getenv("PATH");
//  if(value == NULL)
//    return 1;
//  printf("PATH->%s\n", value);
//  return 0;
//}
////int main(int argc, char* argv[], char*env[])
//{
//  (void)argc;
//  (void)argv;
//  for(int i = 0; env[i];  i++)
//  {
//    printf("env[%d]->%s\n", i, env[i]);
//  }
//  return 0;
//}
//int main(int argc, char* argv[])
//{
//  if(argc != 2){
//    printf("Usage: %s [-a|-b|-c]\n", argv[0]);
//    return 1;
//  }
//  const char* arg = argv[1];
//
//  if(strcmp(arg, "-a") == 0)
//    printf("这是功能1\n");
//  else if(strcmp(arg, "-b") == 0)
//    printf("这是功能2\n");
//  else if(strcmp(arg, "-c") == 0)
//    printf("这是功能3\n");
//
//  else 
//    printf("Usage: %s [-a|-b|-c]\n", argv[0]);
//  return 0;
//}


//int main(int argc, char* argv[])
//{
//  for(int i = 0; i < argc; i++)
//  {
//    printf("%s\n", argv[i]);
//  }
//  return 0;
//}
