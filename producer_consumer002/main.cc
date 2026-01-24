#include "BlockQueue.hpp"
#include <iostream>
#include <unistd.h>
#include "Task.hpp"

// void * consumer(void * args)
// {
//     BlockQueue<int>* bq  = static_cast<BlockQueue<int>*>(args);
//     while (true)
//     {
//         int data = bq->Pop();
//         std::cout << "消费了一个数据：" << data << std::endl;
//     }
// }

// void * productor(void * args)
// {
//     int data  = 1;
//     BlockQueue<int>* bq  = static_cast<BlockQueue<int>*>(args);
//     while (true)
//     {
//         sleep(1);
//         std::cout << "生产了一个数据：" << data << std::endl;
//         bq->Equeue(data);
//         data++;
//     }
// }

void *consumer(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        sleep(1);
        Task t = bq->Pop();

        t.Excute();

        std::cout << "消费了一个任务：" << t.x() << "+" << t.y() << "=" << t.Result() << std::endl;
    }
}

void *productor(void *args)
{
    int x = 1;
    int y = 1;
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        Task t(x, y);
        std::cout << "生产了一个任务：" << x << "+" << y << "=?" << std::endl;
        bq->Equeue(t);

        x++, y++;
    }
}

int main()
{
    // 申请阻塞队列
    BlockQueue<int> *bq = new BlockQueue<int>();

    // 构建生产者消费者
    pthread_t c, p;

    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&p, nullptr, productor, bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}