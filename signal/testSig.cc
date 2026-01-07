#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handlerSig(int sig)
{
    std::cout << "获得了一个信号" << sig << std::endl;
    exit(13);
}
int main()
{
    for (int i = 1; i < 32; i++)
    {
        signal(i, handlerSig);
    }

    int cnt = 0;
    while (true)
    {
        std::cout << "hello world " << cnt++ << "pid:" << getpid() << std::endl;
        sleep(3);
        int a = 10;
        // a /= 0; // 除0错误
        int *p = nullptr;
        *p = 100;
    }
    return 0;
}