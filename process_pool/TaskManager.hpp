// 管理任务
#pragma once

#include <iostream>
#include <vector>
#include <ctime>

typedef void (*task_t)();

void PrintLog()
{
    std::cout << "我是一个打印日志的任务" << std::endl;
}

void Download()
{
    std::cout << "我是一个下载的任务" << std::endl;
}

void Upload()
{
    std::cout << "我是一个上传的任务" << std::endl;
}

class TaskManager
{
private:
    std::vector<task_t> tasks_;

public:
    TaskManager()
    {
        // 种下随机数种子
        srand(time(nullptr));
    }
    void Register(task_t t)
    {
        tasks_.push_back(t);
    }
    int Code()
    {
        // 随机返回任务码
        return rand() % tasks_.size();
    }
    void Execute(int code)
    {
        if (code >= 0 && code < tasks_.size())
        {
            tasks_[code]();
        }
    }
    ~TaskManager() {}
};
