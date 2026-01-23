#include <iostream>
#include <pthread.h>
#include<string>
#include <vector>
#include <unistd.h>

# define NUM 5
int cnt = 1000;

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER; // 定义锁
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;   // 定义条件变量

void* thread_run(void * args)
{
    std::string name = static_cast<const char*> (args);
    while(true)
    {
        // 步骤1：线程申请并拿到锁（glock），进入临界区
        pthread_mutex_lock(&glock);
        
        // 步骤2：调用pthread_cond_wait，核心原子操作：
        // ① 自动释放已经持有的glock锁（归还锁）
        // ② 线程进入gcond的等待队列，阻塞挂起（不再抢锁）
        // 这两步是原子的，不会出现“释放锁但没进入队列”的中间状态
        pthread_cond_wait(&gcond, &glock);
        
        // 步骤3：主线程调用pthread_cond_signal，该线程被唤醒（从等待队列移出）；
        // 步骤4：被唤醒的线程会立刻尝试重新竞争glock锁：
        // - 如果竞争到锁：pthread_cond_wait函数返回，线程走到步骤5；
        // - 如果没竞争到：线程继续阻塞在pthread_cond_wait这一行，直到抢到锁；
        // 步骤5：线程继续执行后续代码（打印、cnt++、解锁）
        // 结论是：pthread_cond_wait是一个阻塞函数—— 线程被唤醒且抢到锁前，
        // 这个函数不会返回；一旦返回，就代表 “锁已重新拿到”，
        // 线程会从pthread_cond_wait的下一行开始执行，而不是重新执行整个thread_run函数。
        std::cout << name << "计算:" << cnt << std::endl;
        cnt ++;
        
        // 步骤4：手动释放锁
        pthread_mutex_unlock(&glock);
    }
    return nullptr;
}

int main()
{
    std::vector<pthread_t> threads;
    for (int i = 0; i < NUM; i++)
    {
        pthread_t tid;
        char * name = new char[64];
        snprintf(name, 64, "thread-%d", i);
        int n = pthread_create(&tid, nullptr, thread_run, name);
        if(n != 0)
            continue;
        threads.push_back(tid);
    }

    sleep(3);

    // 每隔一秒唤醒一个线程
    while (true)
    {
        std::cout <<  "唤醒一个线程..." << std::endl;
        pthread_cond_signal(&gcond);
        // std::cout <<  "唤醒所有线程..." << std::endl;
        // pthread_cond_broadcast(&gcond);
        sleep(1);
    }
    

    for(auto &id : threads)
    {
        int m = pthread_join(id, nullptr);
        (void)m;
    }
    
    return 0;
}