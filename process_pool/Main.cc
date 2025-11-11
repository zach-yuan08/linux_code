#include"ProcessPool.hpp"

int main()
{
    // 创建进程池
    ProcessPool pp(PROCESS_NUM);

    // 启动进程池
    pp.Start();

    // 自动派发任务
    int cnt = 10;
    while (cnt--)
    {
        pp.Run();
        sleep(1);
    }

    // 回收
    pp.Stop();
    return 0;
}

