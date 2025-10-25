#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int gval = 100;

int main()
{
    printf("父进程开始运行,pid: %d\n", getpid());
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return 1;
    }
    else if(id == 0)
    {
        printf("子进程, pid: %d, ppidid: %d, gval: %d, &gval: %p\n", getpid(), getppid(), gval, &gval);
        // child
        while(1)
        {
            sleep(1);
            gval+=10; // 修改
            printf("子进程, pid: %d, ppidid: %d, gval: %d, &gval: %p\n", getpid(), getppid(), gval, &gval);
        }
    }
    else
    {
        //father
        while(1)
        {
            sleep(1);
            printf("父进程, pid: %d, ppidid: %d, gval: %d, &gval: %p\n", getpid(), getppid(), gval, &gval);
        }
    }
    printf("进程开始运行,pid: %d\n", getpid());
}

