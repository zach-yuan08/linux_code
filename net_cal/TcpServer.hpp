#include "Socket.hpp"
#include <iostream>
#include <memory>
#include <sys/wait.h>
#include <functional>

using namespace SocketModule;
using namespace LogModule;

using ioservice_t = std::function<void(std::shared_ptr<Socket> &sock, InetAddr &client)>;

class TcpServer
{
public:
    TcpServer(uint16_t port, ioservice_t service) : _port(port),
                                                    _listensockptr(std::make_unique<TcpSocket>()),
                                                    _isrunning(false),
                                                    _service(service)
    {
        _listensockptr->BuildTcpSocketMethod(_port);
    }

    void Start()
    {
        _isrunning = true;
        while (_isrunning)
        {
            InetAddr client;
            auto sock = _listensockptr->Accept(&client);
            if (sock == nullptr)
            {
                continue;
            }
            LOG(LogLevel ::DEBUG) << "accept success..." << client.StringAddr();

            pid_t id = fork();
            if (id < 0)
            {
                LOG(LogLevel::FATAL) << "fork error";
                exit(FORK_ERR);
            }
            else if (id == 0)
            {
                _listensockptr->Close();
                if (fork() > 0)
                    exit(0);
                _service(sock, client);
                exit(OK);
            }
            else
            {
                sock->Close();
                pid_t rid = ::waitpid(id, nullptr, 0);
                (void)rid;
            }
        }
        _isrunning = false;
    }
    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockptr;
    bool _isrunning;
    ioservice_t _service;
};