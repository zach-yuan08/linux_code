#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include "comm.hpp"
#include <unistd.h>
#include <fcntl.h>

#define FIFO_FILE "fifo"
#define PATH "."
#define FILENAME "fifo"
#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

class NamedFifo
{
private:
    std::string _path;
    std::string _name;
    std::string _fifoname;

public:
    NamedFifo(const std::string &path, const std::string &name)
        : _path(path), _name(name)
    {
        _fifoname = _path + "/" + _name;
        umask(0);                        // 设置权限掩码
        int n = mkfifo(FIFO_FILE, 0666); // 创建管道文件，并设置权限
        if (n < 0)
        {
            ERR_EXIT("mkfile");
        }
        else
        {
            std::cout << "mkfifo sucess" << std::endl;
        }
    }
    ~NamedFifo()
    {
        int n = unlink(_fifoname.c_str());
        if (n == 0)
        {
            ERR_EXIT("unlink");
        }
        else
        {
            std::cout << "remove fifo failed" << std::endl;
        }
        return;
    }
};

class FileOper
{
private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
    int _fd;

public:
    FileOper(const std::string &path, const std::string &name)
        : _path(path), _name(name)
    {
        _fifoname = _path + "/" + _name;
    }

    void OpenForRead()
    {
        _fd = open(_fifoname.c_str(), O_RDONLY);
        if (_fd < 0)
        {
           ERR_EXIT("open");
        }
        else
        {
            std::cout << "open fifo success" << std::endl;
        }
    }

    void OpenForWrite()
    {
        _fd = open(_fifoname.c_str(), O_WRONLY);
        if (_fd < 0)
        {
            ERR_EXIT("open");
        }
        else
        {
            std::cout << "open fifo success" << std::endl;
        }
    }

    void Write()
    {
        std::string message;
        int cnt = 1;
        pid_t id = getpid();
        while (true)
        {
            std::cout << "Please Enter#" << std::endl;
            std::getline(std::cin, message);
            message += (", message number:" + std::to_string(cnt) + ",[" + std::to_string(id) + "]");
            write(_fd, message.c_str(), message.size()); // c_str方法 就是拿到string中的数据
        }
    }

    void Read()
    {
        while (true)
        {
            char buffer[1024];
            int number = read(_fd, buffer, sizeof(buffer) - 1);
            if (number > 0)
            {
                buffer[number] = 0;                               // 第number个位置充当\0
                std::cout << "Clent Say#" << buffer << std::endl; // 打印读到的内容
            }
            else if (number == 0)
            {
                std::cout << "client quit!me too" << number << std::endl;
                break;
            }
            else
            {
                std::cerr << "read error" << std::endl;
                break;
            }
        }
    }
    void Close()
    {
        if (_fd > 0)
            close(_fd);
    }

    ~FileOper()
    {
    }
};
