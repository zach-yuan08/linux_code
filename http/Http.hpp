#pragma once

#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Util.hpp"
#include "Log.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <unordered_map>

using namespace SocketModule;
using namespace LogModule;

const std::string gspace = " ";
const std::string glinespace = "\r\n";
const std::string glinesep = ": ";

const std::string webroot = "./wwwroot";
const std::string homepage = "index.html";

class HttpRequest
{
public:
    HttpRequest()
    {
    }

    std::string Serialize()
    {
        return std::string();
    }
    void ParseReqLine(std::string &reqline)
    {
        std::stringstream ss(reqline);
        ss >> _method >> _uri >> _version;
    }

    bool Deserialize(std::string &reqstr)
    {
        // 1. 提取请求中的请求行
        std::string reqline;
        bool res = Util::ReadOneLine(reqstr, &reqline, glinespace);
        LOG(LogLevel::DEBUG) << reqline;

        // 2. 对请求行反序列化
        ParseReqLine(reqline);

        LOG(LogLevel::DEBUG) << "_method:" << _method;
        LOG(LogLevel::DEBUG) << "_uri:" << _uri;
        LOG(LogLevel::DEBUG) << "_version:" << _version;

        if (_uri == "/")
            _uri = webroot + _uri + homepage;
        else
            _uri = webroot + _uri;
        return true;
    }

    std::string Uri() { return _uri; }
    ~HttpRequest()
    {
    }

private:
    std::string _method;
    std::string _uri;
    std::string _version;
    std::unordered_map<std::string, std::string> _headers;
    std::string _blankline;
    std::string _text;
};

class HttpResonse
{
public:
    HttpResonse() : _blankline(glinespace)
    {
    }

    std::string Serialize()
    {
        std::string status_line = _version + gspace + std::to_string(_code) + gspace + _desc + glinespace;
        std::string resp_header;
        for (auto &header : _headers)
        {
            std::string line = header.first + glinesep + header.second + glinespace;
            resp_header += line;
        }

        return status_line + resp_header + _blankline + _text;
    }

    bool Deserialize(std::string &reqstr)
    {
        return true;
    }

    ~HttpResonse()
    {
    }

public:
    std::string _version;
    int _code;
    std::string _desc;
    std::unordered_map<std::string, std::string> _headers;
    std::string _blankline;
    std::string _text;
};

class Http
{
public:
    Http(uint16_t port) : tsvrp(std::make_unique<TcpServer>(port))
    {
    }

    void HandlerHttpRequest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {
        std::string httpreqstr;
        int n = sock->Recv(&httpreqstr); // 不能保证读到一个完整报文...
        if (n > 0)
        {
            HttpRequest req;
            req.Deserialize(httpreqstr);

            std::string filename = req.Uri();
            HttpResonse resp;
            resp._version = "HTTP/1.1";
            resp._code = 200;
            resp._desc = "OK";

            LOG(LogLevel ::DEBUG) << "用户请求：" << filename;
            bool res = Util::ReadFileContent(filename, &(resp._text));
            (void)res;
            std::string response_str = resp.Serialize();
            sock->Send(response_str);
        }

#ifdef DEBUG
        std::string httpreqstr;
        sock->Recv(&httpreqstr);
        std::cout << httpreqstr;

        HttpResonse resp;
        resp._version = "HTTP/1.1";
        resp._code = 200;
        resp._desc = "OK";

        std::string filename = webroot + homepage;
        bool res = Util::ReadFileContent(filename, &(resp._text));
        (void)res;

        std::string response_str = resp.Serialize();
        sock->Send(response_str);
#endif
    }

    void Start()
    {
        tsvrp->Start([this](std::shared_ptr<Socket> &sock, InetAddr &client)
                     { this->HandlerHttpRequest(sock, client); });
    }

    ~Http()
    {
    }

private:
    std::unique_ptr<TcpServer> tsvrp;
};
