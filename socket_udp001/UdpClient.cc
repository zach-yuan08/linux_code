#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

// 使用格式： ./upclient server_ip server_port
int main(int argc, char *argv[])
{
    // 预备工作
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    // 访问服务器
    // 1. 创建套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std ::endl;
        return 2;
    }

    // 2. 2.1绑定。意义是将本地的ip，port和上面的文件关联。
    // client不需要显示绑定，首次发送消息，OS会自动给client进行绑定，
    // 操作系统知道客户端IP，端口号随机分配 。
    // 不显示绑定的原因：为了避免client端口冲突。
    // client的端口号是几不重要，只要是唯一的就行。
    // 2.2 填写服务器信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());

    while (true)
    {
        std::string input;
        std::cout << "Please inpute: ";
        std::getline(std::cin, input);
        int n = sendto(sockfd, input.c_str(), input.size(), 0, (struct sockaddr *)&server, sizeof(server));
        (void)n;

        char buffer[1024];
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int m = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
        if (m > 0)
        {
            buffer[m] = 0;
            std::cout << buffer << std::endl;
        }
    }

    return 0;
}