#ifndef _THREAD_H_
#define _THREAD_H_

#include <iostream>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <cstring>
#include <functional>

namespace ThreadModlue
{
    static uint32_t number = 1; // bug

    class Thread
    {
        using func_t = std::function<void()>; // 暂时这样写，完全够了
    private:
        void EnableDetach()
        {
            std::cout << "线程被分离了" << std::endl;
            _isdetach = true;
        }
        void EnableRunning()
        {
            _isrunning = true;
        }
        static void *Routine(void *args) // 属于类内的成员函数，默认包含this指针！
        {
            Thread *self = static_cast<Thread *>(args);
            self->EnableRunning();
            if (self->_isdetach)
                self->Detach();
            pthread_setname_np(self->_tid, self->_name.c_str());
            self->_func(); // 回调处理

            return nullptr;
        }
        // bug
    public:
        Thread(func_t func)
            : _tid(0),
              _isdetach(false),
              _isrunning(false),
              res(nullptr),
              _func(func)
        {
            _name = "thread-" + std::to_string(number++);
        }
        void Detach()
        {
            if (_isdetach)
                return;
            if (_isrunning)
                pthread_detach(_tid);
            EnableDetach();
        }

        bool Start()
        {
            if (_isrunning)
                return false;
            int n = pthread_create(&_tid, nullptr, Routine, this);
            if (n != 0)
            {
                std::cerr << "create thread error: " << strerror(n) << std::endl;
                return false;
            }
            else
            {
                std::cout << _name << " create success" << std::endl;
                return true;
            }
        }
        bool Stop()
        {
            if (_isrunning)
            {
                int n = pthread_cancel(_tid);
                if (n != 0)
                {
                    std::cerr << "create thread error: " << strerror(n) << std::endl;
                    return false;
                }
                else
                {
                    _isrunning = false;
                    std::cout << _name << " stop" << std::endl;
                    return true;
                }
            }
            return false;
        }
        void Join()
        {
            if (_isdetach)
            {
                std::cout << "你的线程已经是分离的了,不能进行join" << std::endl;
                return;
            }
            int n = pthread_join(_tid, &res);
            if (n != 0)
            {
                std::cerr << "create thread error: " << strerror(n) << std::endl;
            }
            else
            {
                std::cout << "join success" << std::endl;
            }
        }
        ~Thread()
        {
        }

    private:
        pthread_t _tid;
        std::string _name;
        bool _isdetach;
        bool _isrunning;
        void *res;
        func_t _func;
    };
}

#endif