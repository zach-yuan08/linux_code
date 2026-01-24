#pragma once
#include <iostream>
#include <string>
#include <queue>

const int defalutcap = 5;

template <typename T>
class BlockQueue
{
private:
    bool IsFull() { return _q.size() >= _cap; }
    bool IsEmpty() { return _q.empty(); }

public:
    BlockQueue(int cap = defalutcap)
        : _cap(cap),
          _csleep_num(0),
          _psleep_num(0)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_full_cond, nullptr);
        pthread_cond_init(&_empty_cond, nullptr);
    }

    void Equeue(const T &in)
    {
        // 生产者调用
        // 先加锁保护临界资源
        pthread_mutex_lock(&_mutex);
        // 再判断阻塞队列是不是满了
        while (IsFull())
        {
            _psleep_num++;
            pthread_cond_wait(&_full_cond, &_mutex);
            _psleep_num--;
        }
        _q.push(in);

        if (_csleep_num > 0)
        {
            pthread_cond_signal(&_empty_cond);
            std::cout << "唤醒生产者" << std::endl;
        }
        pthread_mutex_unlock(&_mutex);
    }

    T Pop()
    {
        // 消费者调用
        pthread_mutex_lock(&_mutex);
        while (IsEmpty())
        {
            _csleep_num++;
            pthread_cond_wait(&_empty_cond, &_mutex);
            _csleep_num--;
        }
        T data = _q.front();
        _q.pop();

        if (_psleep_num > 0)
        {
            pthread_cond_signal(&_full_cond);
            std::cout << "唤醒消费者" << std::endl;
        }
        pthread_mutex_unlock(&_mutex);
        return data;
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_full_cond);
        pthread_cond_destroy(&_empty_cond);
    }

private:
    std::queue<T> _q; // 临界资源
    int _cap;         // 容量

    pthread_mutex_t _mutex;     // 保护临界资源
    pthread_cond_t _full_cond;  // 生产者等待队列的条件变量
    pthread_cond_t _empty_cond; // 消费者等待队列的条件变量

    int _csleep_num;
    int _psleep_num;
};
