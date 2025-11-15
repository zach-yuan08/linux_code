// 解释：
// 1 .hpp 的意思是头文件和源文件写在一起，好处是方便，多用于开源项目
#ifndef __PROCESS_POOL_HPP__
#define __PROCESS_POOL_HPP__
// 2 这是 C/C++ 中最常用的头文件保护方式，用于防止头文件被重复包含而导致的编译错误。
// 另一种方式是使用 #pragma once（但 #pragma once 不是标准 C++ 语法，部分编译器可能不支持，而 #ifndef 是标准且跨编译器兼容的）

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "TaskManager.hpp"

// 管道类，用于描述管道
class Channel
{
private:
    int wfd_;          // 父进程（写端）的文件描述符（write file descriptor）
    pid_t subid_;      // 子进程id
    std::string name_; //  子进程名字

public:
    Channel(int fd, pid_t id) : wfd_(fd), subid_(id)
    {
        // 字符串拼接，to_string 将基本数据类型转换为 std::string 类型
        name_ = "channle-" + std::to_string(wfd_) + std::to_string(subid_);
    }

    // 向子进程发送任务码
    void Send(int code)
    {
        // 向wfd_文件中写code
        int n = write(wfd_, &code, sizeof(code));
        (void)n;
    }
    void Close()
    {
        // 关闭该管道的写端，关键是“该”，每个管道都有对应的写端，每个子进程都有自己的wfd_
        close(wfd_);
    }
    void Wait()
    {
        pid_t rid = waitpid(subid_, nullptr, 0);
        (void)rid;
    }
    int Fd() { return wfd_; }
    pid_t SubId() { return subid_; }
    std::string Name() { return name_; }

    ~Channel() {}
};

// 管道管理
class ChannelManager
{
private:
    std::vector<Channel> channels_;
    int next_; // 用于轮询管道
public:
    ChannelManager() : next_(0) {}

    void Inset(int wfd, pid_t subid)
    {
        // emplace_back 在容器尾部原地构造对象，避免多余的拷贝或移动，效率更高，是 push_back 的现代替代方案
        channels_.emplace_back(wfd, subid);
    }
    // 均衡负载选择进程，返回值是引用
    Channel &Select()
    {
        auto &c = channels_[next_];
        next_++;
        next_ %= channels_.size();
        return c;
    }
    void PrintChannel()
    {
        for (auto &channel : channels_)
        {
            std::cout << channel.Name() << std::endl;
        }
    }
    void StopSubProcess()
    {
        for (auto &channel : channels_)
        {
            channel.Close();
            std::cout << "关闭：" << channel.Name() << std::endl;
        }
    }
    void WaitSubProcess()
    {
        for (auto &channel : channels_)
        {
            channel.Wait();
            std::cout << "回收：" << channel.Name() << std::endl;
        }
    }
    ~ChannelManager() {}
};

const int PROCESS_NUM = 5;

// 进程池
class ProcessPool
{
private:
    int process_num_;   // 进程管理，表示此时有多少进程
    ChannelManager cm_; // 管道管理
    TaskManager tm_;    // 管理任务

public:
    ProcessPool(int num) : process_num_(num)
    {
        tm_.Register(PrintLog);
        tm_.Register(Download);
        tm_.Register(Upload);
    }

    // 子进程从文件描述符rfd（读端）读取任务码，做相应的任务
    void Work(int rfd)
    {
        // 只要父进程不退出，子进程就一直读
        while (true)
        {
            int code = 0;
            // 从文件描述符 rfd（管道读端）中读取一个整数任务码到 code。
            ssize_t n = read(rfd, &code, sizeof(code));
            if (n > 0)
            {
                // 判断是否成功读到一个完整的任务码（sizeof(code)）
                if (n != sizeof(code))
                {
                    continue;
                }
                // 打印调试信息显示子进程 PID 和收到的任务码。
                std::cout << "子进程[" << getpid() << "]收到一个任务码：" << code << std::endl;
                // do
                tm_.Execute(code);
            }
            else if (n == 0)
            {
                std::cout << "子进程退出" << std::endl;
                break;
            }
            else
            {
                std::cout << "读取错误" << std::endl;
                break;
            }
        }
    }
    // 启动进程池
    bool Start()
    {
        for (int i = 0; i < process_num_; i++)
        {
            // 创建管道
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
                return false;

            // 创建子进程
            pid_t subid = fork();
            if (subid < 0)
                return false;
            else if (subid == 0)
            {
                // 子进程
                close(pipefd[1]);

                // do
                Work(pipefd[0]);
                close(pipefd[0]);
                exit(0);
            }
            else
            {
                // 父进程
                close(pipefd[0]);

                // do
                cm_.Inset(pipefd[1], subid);
            }
        }
        return true;
    }

    void Run()
    {
        // 选择一个任务
        int task_code = tm_.Code();

        // 选择一个信道，负载均衡的选择子进程，完成任务
        auto &c = cm_.Select();

        // 发送任务
        c.Send(task_code);
        std::cout << "发送了一个任务码：" << task_code << std::endl;
    }

    void Stop()
    {
        cm_.StopSubProcess();
        cm_.WaitSubProcess();
    }

    //
    ~ProcessPool() {}
};

#endif
