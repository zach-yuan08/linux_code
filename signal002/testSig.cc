#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <vector>

void Sched()
{
    std::cout << "我是进程调度" << std::endl;
}

void MemManger()
{
    std::cout << "我是周期性管理内存，正在检查有没有内存问题" << std::endl;
}

void Fflush()
{
    std::cout << "我是刷新程序，我正在定期刷新内存数据到磁盘" << std::endl;
}

using func_t = std::function<void()>;

std::vector<func_t> funcs;

void handlerSig(int sig)
{
    std::cout << "#######################" << std::endl;
    for (auto f : funcs)
        f();
    std::cout << "#######################" << std::endl;
    alarm(1);
}

int main()
{
    funcs.push_back(Sched);
    funcs.push_back(MemManger);
    funcs.push_back(Fflush);

    signal(SIGALRM, handlerSig);
    alarm(1);
    while (true)
    {
        pause();
    }

    return 0;
}

// void handlerSig(int sig)
// {
//     std::cout << "收到一个信号：" << sig << std::endl;
//     alarm(1);
// }

// int main()
// {
//     signal(SIGALRM, handlerSig);
//     alarm(1);
//     while (true)
//     {
//         std :: cout<< ".," << "pid:" << getpid() << std::endl;
//         sleep(1);
//     }
//     return  0;
// }

// int main()
// {
//     for (int i = 0; i < 32; i++)
//     {
//         signal(i, handlerSig);
//     }

//     alarm(1); // 设置闹钟1s后响，1s之后当前进程会收到一个信号

//     while (true)
//     {
//         // std::cout << "count: " << cnt++ << std::endl;
//         cnt++;
//     }

//     return 0;
// }