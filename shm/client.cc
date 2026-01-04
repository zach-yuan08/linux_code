#include "Shm.hpp"

int main()
{
    // 获取共享内存
    Shm shm(pathname, projid, USER);

    // 获取共享内存虚拟起始地址
    char* mem = (char*)shm.VirtualAddr();

    // 使用共享内存
    // 每隔一秒向共享内存写入数据
    for(char c = 'A'; c <= 'Z'; c++)
    {
        mem[c - 'A'] = c;
        sleep(1);
    }

    return 0;
}