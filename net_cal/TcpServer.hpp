#include "Socket.hpp"
#include <iostream>
#include <memory>

using namespace SocketModule;
using namespace LogModule;

class TcpServer
{
public:
    TcpServer(uint16_t port) : _port(port),
                               _listensockptr(std::make_unique<TcpSocket>())
    {
        _listensockptr->BuildTcpSocketMethod(_port);
    }

    void Start()
    {
        _isrunning = true;
        while (_isrunning)
        {
            sleep(1);
            _listensockptr->Accept();
            LOG(LogLevel ::DEBUG) << "server is running...";
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
};
