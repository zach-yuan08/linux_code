#include "TcpServer.hpp"
#include <memory>

void Usage(std::string proc)
{
    std::cerr << "Usage: " << proc << "port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(std::stoi(argv[1]));
    tsvr->Start();

    return 0;
}
