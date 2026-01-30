#include <iostream>
#include <memory>
#include "UdpServer.hpp"
#include "Route.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);
    Enable_Console_Log_Strategy();

    Route r;

    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port, [&r](int sockfd, const std::string &message, InetAddr &peer)
                                                                  { r.MessageRoute(sockfd, message, peer); });

    usvr->Init();
    usvr->Start();

    return 0;
}