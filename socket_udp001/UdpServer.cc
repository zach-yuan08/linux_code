#include <iostream>
#include <memory>
#include "UdpServer.hpp"

// 使用格式 ./udpserver ip port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return 1;
    }

    // std::string ip = argv[1];
    uint16_t port = std::stoi(argv[1]);
    Enable_Console_Log_Strategy();

    std::unique_ptr<UdpServer> user = std::make_unique<UdpServer>(port);
    user->Init();
    user->Start();

    return 0;
}