#pragma once
#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"

using namespace LogModule;
const static int defaultsockfd = -1;
const static int backlog = 8;
class TcpServer : public NoCopy
{
public:
    TcpServer(uint16_t port) : _port(port), _listensockfd(defaultsockfd), _isrunning(false)
    {
    }

    void Init()
    {
        // 回收子进程做法1. 
        // signal(SIGCHLD, SIG_IGN); // 父进程忽略子进程信号，让内核帮着回收子进程
        // 1. 创建套接字，socket类比open
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket error";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel ::INFO) << "socket success: " << _listensockfd;

        // 2. bind端口号
        InetAddr local(_port);
        int n = bind(_listensockfd, local.NetAddrPtr(), local.NetAddrLen());
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bing error";
            exit(BIND_ERR);
        }
        LOG(LogLevel ::INFO) << "bind success: " << _listensockfd;

        // 3. 设置socket状态
        n = listen(_listensockfd, backlog);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "listen error";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel ::INFO) << "listen success: " << _listensockfd;
    }
    
    void Service(int sockfd, InetAddr &peer)
    {
        char buffer[1024];
        while (true)
        {
            // 1. 先读取数据
            ssize_t n = read(sockfd, buffer, sizeof(buffer - 1));
            if (n > 0)
            {
                // 给buffer设置成C风格的字符串
                buffer[n] = 0;
                LOG(LogLevel::DEBUG) << peer.StringAddr() << "say# " << buffer;
                // 2. 写回数据
                std::string echo_string = buffer;
                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if (n == 0)
            {
                LOG(LogLevel::DEBUG) << peer.StringAddr() << "退出了...";
                close(sockfd);
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << peer.StringAddr() << "异常...";
                close(sockfd);
                break;
            }
        }
    }

    void Run()
    {
        _isrunning = true;
        while (_isrunning)
        {
            // 获取链接 ：其实内核已经建立连接了，获取链接得意义是让程序员知道服务器被谁链接，方便通信
            struct sockaddr_in peer;
            socklen_t len = sizeof(sockaddr_in);
            int sockfd = accept(_listensockfd, CONV(peer), &len);
            if (sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue;
            }
            InetAddr addr(peer);
            LOG(LogLevel ::INFO) << "accept success, peer addr: " << addr.StringAddr();

            // Service(sockfd, addr);
            pid_t id = fork();
            if(id < 0)
            {
                LOG(LogLevel::FATAL) << "fork error";
                exit(FORK_ERR);
            }
            else if(id == 0) // 子进程
            {
                close(_listensockfd); // 关掉不想让子进程访问的套接字，关闭文件描述符
                // 回收子进程方法2：
                if(fork() > 0) // 子进程再次创建子进程，也就是让孙子进程去执行业务
                    exit(OK);

                Service(sockfd, addr); //孙子进程处理业务，孙子进程变成了孙子进程，被1号进程领养，被系统回收
                exit(OK);
            }
            else
            {
                close(sockfd); // 父进程管监听就行，不用管其他的套接字。
                pid_t rid = waitpid(id, nullptr, 0); // 回收子进程，采用方法2已经不会再阻塞，因为子进程瞬间退出了
                (void)rid;
            }
        }
        _isrunning = false;
    }

    ~TcpServer() {}

private:
    uint16_t _port;
    int _listensockfd;
    bool _isrunning;
};
