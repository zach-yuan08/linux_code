#include "Shm.hpp"

int main()
{
    // 创建共享内存
    Shm shm(pathname, projid, CREATER);

    // 获取共享内存虚拟起始地址
    char *mem = (char *)shm.VirtualAddr();

    // 使用共享内存
    // 每隔一秒从共享内存中读数据
    while (true)
    {
        printf("%s\n", mem);
        sleep(1); 
    }
    return 0;
}