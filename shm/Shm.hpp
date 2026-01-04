#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

const int gdefaultid = -1;
const int gsize = 4096;
const std::string pathname = ".";
const int projid = 0x66;
const int gmode = 0666;

#define CREATER "creater"
#define USER "user"
#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

class Shm
{
private:
    // 直接将公共部分抽象出来，只有参数不相同
    void CreatHeaper(int flg)
    {
        _shmid = shmget(_key, _size, flg);
        if (_shmid < 0)
        {
            ERR_EXIT("shmget");
        }
        printf("shmid: %d\n", _shmid);
    }

    void Creat()
    {
        CreatHeaper(IPC_CREAT | IPC_EXCL | gmode);
    }

    void Get()
    {
        CreatHeaper(IPC_CREAT);
    }

    void Attach()
    {
        // 挂接共享内存
        _start_mem = shmat(_shmid, nullptr, 0);
        // 判断是否挂接成功，成功后返回起始虚拟地址
        if ((long long)_start_mem < 0)
        {
            ERR_EXIT("shmat");
        }
        printf("attach success!\n");
    }

    void Destroy()
    {
        // 共享内存没有被成功建立
        if (_shmid == gdefaultid)
            return;

        // 成功建立后的删除
        int n = shmctl(_shmid, IPC_RMID, nullptr);
        // 删除成功
        if (n > 0)
        {
            printf("shmctl delete shm: %d success!\n", _shmid);
        }
        // 删除失败
        else
        {
            ERR_EXIT("shmctl");
        }
    }

public:
    Shm(const std::string &pathname, int projid, const std::string &usertype)
        : _shmid(gdefaultid),
          _size(gsize),
          _start_mem(nullptr),
          _usertype(usertype)
    {
        _key = ftok(pathname.c_str(), projid);
        if (_key < 0)
        {
            ERR_EXIT("ftok");
        }
        printf("key: 0x%x\n", _key);

        if (_usertype == CREATER)
            Creat();
        else if (_usertype == USER)
            Get();
        else
        {
        }
        Attach();
    }

    void *VirtualAddr()
    {
        printf("VirtualAddr:%p\n", _start_mem);
        return _start_mem;
    }

    int Size()
    {
        return _size;
    }

    ~Shm()
    {
        if (_usertype == CREATER)
            Destroy();
    }

private:
    int _shmid;
    int _size;
    void *_start_mem; // 起始的虚拟地址
    std::string _usertype;
    key_t _key;
};