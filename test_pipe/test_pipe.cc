#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void child_write(int wfd)
{
    char buffer[1024];
    int cnt = 0;
    while (true)
    {
        // 将信息格式化输入到buffer中
        snprintf(buffer, sizeof(buffer), "I am child, pid: %d, cnt = %d", getpid(), cnt++); 
        // 向文件中写入，记住！管道也是文件，所以和读写文件的方式一摸一样。
        // 向wfd（管道）中写buffer，长度为strlen(buffer)
        write(wfd, buffer, strlen(buffer));
        // sizeof 用来获取buffer总容量，strlen来计算字符串有效长度

        // 每隔一秒写一次
        sleep(1);
    }
}

void father_read(int rfd)
{
    char buffer[1024];
    while (true)
    {
        buffer[0] = 0;
        // 从特定的文件描述符指向的文件中读数据，读到buffer中，读取的大小是sizeof(buffer) - 1
        // 细节：写文件的时候，用的是strlen来判断长度，读文件的时候用sizeof
        // 写文件时把文件内容当字符串完整的写过来，strlen是不计算 \0 的。
        // 读文件时，认为读取的是字符串，所以要预留一个位置放 \0
        // 向文件中写入内容时，不用把 \0 写入文件，\0是c语言的内容，和文件没有关系
        // 一句话总结：接口是为C语言提供，会自动添加\0，而系统调用，偏底层，是不关心 \0 
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
    int fds[2] = {0}; // 创建文件描述符，fdas[0]读端 fdas[1]写端
    int n = pipe(fds); // 创建管道，以管道文件的形式打开文件，并且分配两个文件描述符
    if (n < 0)
    {
        std::cout << "pipe error" << std::endl;
        return 1;
    }
    std::cout << "fds[0]:" << fds[0] << std::endl; // 3
    std::cout << "fds[1]:" << fds[1] << std::endl; // 4

    // 2. 创建子进程
    pid_t id = fork();
  
    if (id == 0)
    {
        // child
        // 3. 关闭不需要读端口，形成通信信道
        close(fds[0]);

        // dowhat
        child_write(fds[1]); // 对“一号文件”写

        // 任务完成，关闭自己的端口
        close(fds[1]);
        exit(0);
    }

    // 关掉父进程不需要的写端
    close(fds[1]); 

    // dowhat
    father_read(fds[0]); // 对“零号”文件读

    // 回收子进程
    waitpid(id, nullptr, 0);
    close(fds[0]);

    return 0;
}