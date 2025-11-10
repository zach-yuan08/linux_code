#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void child_write(int wfd)
{
    char buffer[1024];
    int cnt = 0;
    while (true)
    {
        snprintf(buffer, sizeof(buffer), "I am child, pid: %d, cnt = %d", getpid(), cnt++);
        write(wfd, buffer, strlen(buffer));
        sleep(1);
    }
}

void father_read(int rfd)
{
    char buffer[1024];
    while (true)
    {
        buffer[0] = 0;
        size_t n = read(rfd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = 0;
            std ::cout << "child say:" << buffer << std::endl;
        }
    }
}
int main()
{
    // 1. 创建管道
    int fds[2] = {0}; // fdas[0]读端 fdas[1]写端
    int n = pipe(fds);
    if (n < 0)
    {
        std::cout << "pipe error" << std::endl;
        return 1;
    }

    std::cout << "fds[0]:" << fds[0] << std::endl;
    std::cout << "fds[1]:" << fds[1] << std::endl;

    // 2. 创建子进程
    pid_t id = fork();
    if (id == 0)
    {
        // child
        // 关闭不需要读端口，形成通信信道
        close(fds[0]);

        // dowhat
        child_write(fds[1]);

        // 任务完成，关闭自己的端口
        close(fds[1]);
        exit(0);
    }

    // 关掉父进程不需要的写端
    close(fds[1]);

    // dowhat
    father_read(fds[0]);

    // 回收子进程
    waitpid(id, nullptr, 0);
    close(fds[0]);

    return 0;
}