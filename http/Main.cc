#include "Http.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        exit(USAGE_ERR);
    }
    uint16_t port = std::stoi(argv[1]);

    std::unique_ptr<Http> httpptr = std::make_unique<Http>(port);
    httpptr->Start();
    
    return 0;
}