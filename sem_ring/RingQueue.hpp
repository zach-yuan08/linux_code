#pragma once

#include <iostream>
#include <vector>
#include "Sem.hpp"
#include "Mutex.hpp"

static const int gcap = 5;

using namespace SemModule;
using namespace MutexModule;

template <typename T>
class RingQueue
{
public:
    RingQueue(int cap = gcap)
    :_rq(cap),
    _cap(cap),
    _blank_sem(cap),
    _p_step(0),
    _data_sem(0),
    _c_step(0)
    {}

    void Equeue(const T & in)
    {
        // 生产者
        // 1. 申请信号量
        _blank_sem.P();
        {
            LockGuard lockguard(_pmutex);
            // 2. 生产
            _rq[_p_step] = in;
            // 3. 更新下标
            _p_step++;
            // 4. 维持环形特性
            _p_step %= _cap;
        }
        _data_sem.V();
    }

    void Pop(T *out)
    {
        // 消费者
        // 1. 申请信号量
        _data_sem.P();
        {
            LockGuard lockguard(_cmutex);
            // 2. 消费
            *out = _rq[_c_step];
            // 3. 更新下标
            _c_step++;
            // 4. 维持环状特性
            _c_step %= _cap;
        }
        _blank_sem.V();
    }
    ~RingQueue();

private:
    std::vector<T> _rq; // 循环队列
    int _cap;           // 容量

    // 生产者
    Sem _blank_sem;     // 生产者可以生产的位置的信号量
    int _p_step;        // 生产者实际生产的位置

    // 消费者
    Sem _data_sem;      // 消费者可以消费的数据的信号量
    int _c_step;        

    // 为了维护多生产多消费，维护cc和pp之间的关系
    Mutex _cmutex;
    Mutex _pmutex;
};
