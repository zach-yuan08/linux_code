#include <iostream>
#include <mutex>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 1000;

class ThreadData
{
public:
    ThreadData(const std::string &n, pthread_mutex_t &lock)
        : name(n),
          lockp(&lock)
    {
    }
    ~ThreadData() {}

    std::string name;
    pthread_mutex_t *lockp;
};

void *route(void *arg)
{
    ThreadData *td = static_cast<ThreadData *>(arg);
    while (1)
    {
        pthread_mutex_lock(td->lockp);
        if (ticket > 0) // 1. 判断
        {
            usleep(1000);                                             // 模拟抢票花的时间
            printf("%s sells ticket:%d\n", td->name.c_str(), ticket); // 2. 抢到了票
            ticket--;                                                 // 3. 票数--
            pthread_mutex_unlock(td->lockp);
        }
        else
        {
            pthread_mutex_unlock(td->lockp);
            break;
        }
    }
    return nullptr;
}

int main(void)
{
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, nullptr); // 初始化锁

    pthread_t t1, t2, t3, t4;
    ThreadData *td1 = new ThreadData("thread 1", lock);
    pthread_create(&t1, NULL, route, td1);

    ThreadData *td2 = new ThreadData("thread 2", lock);
    pthread_create(&t2, NULL, route, td2);

    ThreadData *td3 = new ThreadData("thread 3", lock);
    pthread_create(&t3, NULL, route, td3);

    ThreadData *td4 = new ThreadData("thread 4", lock);
    pthread_create(&t4, NULL, route, td4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}

// int ticket = 1000;
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// void *route(void *arg)
// {
//     char *id = (char *)arg;
//     while (1)
//     {
//         pthread_mutex_lock(&lock);
//         if (ticket > 0) // 1. 判断
//         {
//             usleep(1000);                               // 模拟抢票花的时间
//             printf("%s sells ticket:%d\n", id, ticket); // 2. 抢到了票
//             ticket--;                                   // 3. 票数--
//             pthread_mutex_unlock(&lock);
//         }
//         else
//         {
//             pthread_mutex_unlock(&lock);
//             break;
//         }
//     }
//     return nullptr;
// }

// int main(void)
// {
//     pthread_t t1, t2, t3, t4;

//     pthread_create(&t1, NULL, route, (void *)"thread 1");
//     pthread_create(&t2, NULL, route, (void *)"thread 2");
//     pthread_create(&t3, NULL, route, (void *)"thread 3");
//     pthread_create(&t4, NULL, route, (void *)"thread 4");

//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);
//     pthread_join(t3, NULL);
//     pthread_join(t4, NULL);
// }