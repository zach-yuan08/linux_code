#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <vector>

// 多线程
int num = 10;
void *routine(void *args)
{
    std::string name = static_cast<const char *>(args);
    int cnt = 5;
    while (cnt--)
    {
        std::cout << "新线程name:" << name << std::endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    std::vector<pthread_t> tids;
    for (int i = 0; i < num; i++)
    {
        pthread_t tid;
        char id[64];
        snprintf(id, sizeof(id), "thread-%d", i);
        int n = pthread_create(&tid, nullptr, routine, id);
        if (n == 0)
            tids.push_back(tid);
        else
            continue;
        sleep(1);
    }

    for (int i = 0; i < num; i++)
    {
        int n = pthread_join(tids[i], nullptr);
        if (n == 0)
            std::cout << "等待新线程成功" << std::endl;
        else
            std::cout << "等待失败" << std::endl;
    }

    return 0;
}

// void *rotine(void *args)
// {
//     std::string name = static_cast<const char *>(args);
//     int cnt = 10;
//     while (cnt--)
//     {
//         std::cout << "新线程name:" << name << std::endl;
//         sleep(1);
//     }
//     return (void *)10;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, rotine, (void *)"thread-1");

//     pthread_detach(tid);
//     std::cout << "新线程被分离" << std::endl;

//     int cnt = 5;
//     while (cnt--)
//     {
//         std::cout << "main线程name：" << std::endl;
//         sleep(1);
//     }

//     int n = pthread_join(tid, nullptr);
//     if(n != 0)
//     {
//         std::cout << "pthread_join error: " << strerror(n) << std::endl;
//     }
//     else
//     {
//         std::cout << "pthread_join success"<< std::endl;
//     }

//     return 0;
// }

// void* rotine(void* args)
// {
//     std::string name = static_cast<const char *>(args);
//     int cnt = 10;
//     while (cnt--)
//     {
//         std::cout << "新线程name:" << name << std::endl;
//         sleep(1);
//     }
//     return (void*)10;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, rotine, (void*)"thread-1");

//     int cnt = 5;
//     while (cnt--)
//     {
//         std::cout << "main线程名字：" << std::endl;
//         sleep(1);
//     }

//     void *ret = nullptr;
//     pthread_join(tid, &ret);

//     std::cout << "新线程结束，退出码：" << (long long)ret << std::endl;
//     return 0;
// }

// void showtid(pthread_t &tid)
// {
//     printf("tid: 0x%lx\n", tid);
// }

// std::string FormatId(const pthread_t &tid)
// {
//     char id[64];
//     snprintf(id, sizeof(id), "0x%lx", tid);
//     return id;
// }

// void *routine(void *args)
// {
//     std::string name = static_cast<const char *>(args);
//     pthread_t tid = pthread_self();
//     int cnt = 5;
//     while (cnt--)
//     {
//         std::cout << "我是一个新线程: 我的名字是: " << name << " 我的Id是: " << FormatId(tid) << std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     int n = pthread_create(&tid, nullptr, routine, (void*)"thread-1");
//     (void)n;
//     showtid(tid);
//     int cnt = 5;
//     while (cnt--)
//     {
//         std::cout << "我是main线程: 我的名字是: main thread" << " 我的Id是: " << FormatId(pthread_self()) << std::endl;
//         sleep(1);
//     }

//     pthread_join(tid, nullptr);

//     return 0;
// }

// void* threadrun(void* args)
// {
//     std::string name = (const char*)args;
//     while (true)
//     {
//         std::cout << "我是新线程：name：" << name << std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadrun, (void*)"thread - 1");

//     while (true)
//     {
//         std::cout << "我是主线程..." << std::endl;
//         sleep(1);
//     }

//     return 0;
// }