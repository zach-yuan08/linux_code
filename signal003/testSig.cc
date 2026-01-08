#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <vector>

void PrintPending(sigset_t &pending)
{
    printf("我是一个进程(%d),pending: ", getpid());
    for (int signo = 31; signo >= 1; signo--)
    {
        if (sigismember(&pending, signo))
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void test()
{
    int a;
    scanf("%d", &a);
    printf("%d\n", a);
}
void handler(int sig)
{
    std::cout << "递达" << sig << "信号" << std::endl;
}
int main()
{
    signal(SIGINT, handler);
    // 1. 屏蔽2号信号
    sigset_t block, oblock;
    sigemptyset(&block); // 清空位图
    sigemptyset(&oblock);
    int cnt = 0;

    // for (int i = 1; i < 32; i++) // 给所有信号屏蔽掉
    //     sigaddset(&block, i);
    sigaddset(&block, SIGINT);
    // 此时并没有对2号信号实现屏蔽，此时的block仅仅是在栈上的变量。没有修改进程PCB。
    int n = sigprocmask(SIG_SETMASK, &block, &oblock); // 这里才修改了PCB
    (void)n;

    // 4. 重复获取并打印
    while (true)
    {
        // 2. 获取pending信号集合
        sigset_t pending;
        int m = sigpending(&pending);

        // 3. 打印
        PrintPending(pending);

        if (cnt == 10)
        {
            // 5.恢复二号信号
            sigprocmask(SIG_SETMASK, &oblock, nullptr);
            std ::cout << "解除对二号信号的屏蔽" << std ::endl;
        }
        cnt++;
        sleep(1);
    }

    return 0;
}