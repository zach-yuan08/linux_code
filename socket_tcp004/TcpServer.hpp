#pragma once
#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"

using namespace LogModule;
using namespace ThreadPoolModule;

using func_t = std::function<std::string(const std::string &, InetAddr &)>;

const static int defaultsockfd = -1;
const static int backlog = 8;

class TcpServer : public NoCopy
{
public:
    TcpServer(uint16_t port, func_t func) : _port(port),
                                            _listensockfd(defaultsockfd),
                                            _isrunning(false),
                                            _func(func)
    {
    }
    void Init()
    {
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket error";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::INFO) << "socket success: " << _listensockfd; // 3

        InetAddr local(_port);
        int n = bind(_listensockfd, local.NetAddrPtr(), local.NetAddrLen());
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bind error";
            exit(BIND_ERR);
        }
        LOG(LogLevel::INFO) << "bind success: " << _listensockfd; // 3

        n = listen(_listensockfd, backlog);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "listen error";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel::INFO) << "listen success: " << _listensockfd; // 3
    }

    class ThreadData
    {
    public:
        ThreadData(int fd, InetAddr &ar, TcpServer *s) : sockfd(fd), addr(ar), tsvr(s)
        {
        }

    public:
        int sockfd;
        InetAddr addr;
        TcpServer *tsvr;
    };

    void Service(int sockfd, InetAddr &peer)
    {
        char buffer[1024];
        while (true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = 0; // 设置为C风格字符串， n<= sizeof(buffer)-1
                LOG(LogLevel::DEBUG) << peer.StringAddr() << " #" << buffer;

                std::string echo_string = _func(buffer, peer);

                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if (n == 0)
            {
                LOG(LogLevel::DEBUG) << peer.StringAddr() << " 退出了...";
                close(sockfd);
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << peer.StringAddr() << " 异常...";
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
            struct sockaddr_in peer;
            socklen_t len = sizeof(sockaddr_in);
            int sockfd = accept(_listensockfd, CONV(peer), &len);
            if (sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue;
            }
            InetAddr addr(peer);
            LOG(LogLevel::INFO) << "accept success, peer addr : " << addr.StringAddr();

            ThreadData *td = new ThreadData(sockfd, addr, this);
            pthread_t tid;
            pthread_create(&tid, nullptr, Routine, td);
        }
        _isrunning = false;
    }
    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    int _listensockfd;
    bool _isrunning;

    func_t _func;
};