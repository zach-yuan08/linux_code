#include <iostream>
#include <memory>
#include "UdpServer.hpp"
#include "Dict.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);
    Enable_Console_Log_Strategy();

    Dict dict;
    dict.LoadDict();

    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port, [&dict](const std::string &word, InetAddr &cli) -> std::string
                                                                  { return dict.Translate(word, cli); });
    usvr->Init();
    usvr->Start();

    return 0;
}