#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
     printf("我是一个进程, pid: %d\n", getpid());

    int x;
    scanf("%d", &x);
    while(1)
    {
        printf("hello linux\n");
    }
    return 0;
}

