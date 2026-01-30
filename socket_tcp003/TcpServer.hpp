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

    class ThreadData
    {
    public:
        ThreadData(int fd, InetAddr &ar, TcpServer *s) : sockfd(fd), addr(ar), tsvr(s)
        {
        }
        int sockfd;
        InetAddr addr;
        TcpServer *tsvr;
    };

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

    static void *Routine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->tsvr->Service(td->sockfd, td->addr);
        delete td;
        return nullptr;
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

            ThreadData *td = new ThreadData(sockfd, addr, this);
            pthread_t tid;
            pthread_create(&tid, nullptr, Routine, td);
        }
        _isrunning = false;
    }

    ~TcpServer() {}

private:
    uint16_t _port;
    int _listensockfd;
    bool _isrunning;
};
