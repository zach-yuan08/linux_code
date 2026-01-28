#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"

using namespace LogModule;
const int defaultfd = -1;
class UdpServer
{

public:
    UdpServer(uint16_t port)
        : _sockfd(defaultfd),
          _port(port)
    {
    }

    void Init()
    {
        // 1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(LogLevel ::FATAL) << "socket error !";
            exit(1);
        }
        LOG(LogLevel::INFO) << "socket success, sockfd: " << _sockfd;

        // 2. 绑定 socket信息，主要是ip和端口号
        // 2.1 先填充结构体，供bind使用
        struct sockaddr_in local;
        bzero(&local, sizeof(local)); // 先清零

        local.sin_family = AF_INET; // 填协议家族

        local.sin_port = htons(_port); // 填端口号，需要将本地主机序列转化为网络序列

        // 填IP，需要先把IP字符串转成四字节格式（32位无符号整数），
        // 然后四字节再转成网络序列，还好库已经提供实现转化的接口
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_addr.s_addr = INADDR_ANY;

        // 2.2 绑定，注意第二个参数，为了更通用所以传基类，所以强转
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));

        // 2.3 判断是否绑定成功
        if (n < 0)
        {
            LOG(LogLevel ::FATAL) << "bind error!";
            exit(2);
        }
        // 绑定成功
        LOG(LogLevel ::INFO) << "bind success, sockfd: " << _sockfd;
    }

    void Start()
    {
        _isrunning = true;
        while (_isrunning)
        {
            // 1. 读取，收消息
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);

            size_t s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);

            if (s > 0)
            {
                int peer_port = ntohs(peer.sin_port);
                std::string peer_ip = inet_ntoa(peer.sin_addr);

                buffer[s] = 0;
                LOG(LogLevel ::DEBUG) << "[" << peer_ip << ":" << peer_port << "]# " << buffer;
                // 2. 响应，发消息
                std::string echo_string = "server echo@ ";
                echo_string += buffer;
                sendto(_sockfd, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr *)&peer, len);
            }
        }
    }

    ~UdpServer()
    {
    }

private:
    int _sockfd;
    uint16_t _port;
    // std::string _ip; // 本地服务器用的是点分十进制风格，字符串
    bool _isrunning;
};
