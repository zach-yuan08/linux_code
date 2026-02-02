#pragma once
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Log.hpp"
#include "Common.hpp"

using namespace LogModule;
const std::string dev = "/dev/null";
// 将服务进行守护进程化的进程
void Daemon(int nochdir, int noclose)
{
    // 1. 忽略IO，子进程退出，等相关信号
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // 2. 父进程直接结束
    if (fork() > 0)
        exit(0);

    // 3. 让当前子进程成为一个独立的会话
    setsid();

    if (nochdir == 0) // 更改进程的工作路径
        chdir("/");

    if (noclose == 0)
    {
        // 4.守护进程不从键盘输入，也不需要向显示器打印
        int fd = ::open(dev.c_str(), O_RDWR);
        if (fd < 0)
        {
            LOG(LogLevel ::FATAL) << "open " << dev << " error";
            exit(OPEN_ERR);
        }
        else
        {
            dup2(fd, 0);
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);
        }
    }
}