#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"

namespace SocketModule
{
    using namespace LogModule;
    const static int gbacklog = 16;
    // 基类套接字
    // 模板方法模式：大部分方法为纯虚方法
    class Socket
    {
    public:
        virtual ~Socket() {};
        virtual void SocketOrDie() = 0;
        virtual void BindOrDie(uint16_t port) = 0;
        virtual void ListenOrDie(int backlog) = 0;
        virtual bool Accept() = 0;

    public:
        void BuildTcpSocketMethod(uint16_t port, int backlog = gbacklog)
        {
            SocketOrDie();
            BindOrDie(port);
            ListenOrDie(backlog);
        }

        // void BuildUdpSocketMethod()
        // {
        //     SocketOrDie();
        //     BindOrDie();
        // }
    };

    class TcpSocket : public Socket
    {
    public:
        ~TcpSocket() {};
        void SocketOrDie() override
        {
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                LOG(LogLevel ::FATAL) << "socket error";
                exit(SOCKET_ERR);
            }
            LOG(LogLevel ::INFO) << "socket success";
        }

        void BindOrDie(uint16_t port) override
        {
            InetAddr localadd(port);
            int n = ::bind(_sockfd, localadd.NetAddrPtr(), localadd.NetAddrLen());
            if (n < 0)
            {
                LOG(LogLevel ::FATAL) << "bind error";
                exit(BIND_ERR);
            }
            LOG(LogLevel ::INFO) << "bind success";
        }

        void ListenOrDie(int backlog) override
        {
            int n = ::listen(_sockfd, backlog);
            if (n < 0)
            {
                LOG(LogLevel ::FATAL) << "listen error";
                exit(LISTEN_ERR);
            }
            LOG(LogLevel ::INFO) << "listen success";
        }

        bool Accept() override
        {
            return true;
        }

    private:
        int _sockfd; // 有可能是listen套接字也有可能是普通的socket
    };

    // class UdpSocket : public Socket
    // {
    // public:
    //     ~UdpSocket() {};
    //     int SocketOrDie() override
    //     {
    //         return 0;
    //     }
    //     bool BindOrDie() override
    //     {
    //         return true;
    //     }
    //     bool ListenOrDie() override
    //     {
    //         return true;
    //     }
    //     bool Accept() override
    //     {
    //         return true;
    //     }

    // private:
    // };

};