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
const std::string page_404 = "/404.html";

class HttpRequest
{
public:
    HttpRequest()
    {
    }

    // 解析请求行
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

        if (_uri == "/") // 如果访问的是根目录，返回首页
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

class HttpResponse
{
public:
    HttpResponse() : _blankline(glinespace), _version("HTTP/1.0")
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

    void SetTargetFile(const std::string &target)
    {
        _targetfile = target;
    }

    void SetCode(int code)
    {
        _code = code;
        switch (_code)
        {
        case 200:
            _desc = "OK";
            break;
        case 404:
            _desc = "NotFound";
            break;
        default:
            break;
        }
    }

    void SetHeader(const std::string &key, const std::string &value)
    {
        auto iter = _headers.find(key);
        if (iter != _headers.end())
            return;
        _headers.insert(std::make_pair(key, value));
    }

    std::string Uri2Suffix(const std::string &targetfile)
    {
        // ./wwwroot/a/b/c.html
        auto pos = targetfile.rfind(".");
        if (pos == std::string::npos)
        {
            return "text/html";
        }

        std::string suffix = targetfile.substr(pos);
        if (suffix == ".html" || suffix == ".htm")
            return "text/html";
        else if (suffix == ".jpg")
            return "image/jpeg";
        else if (suffix == "png")
            return "image/png";
        else
            return "";
    }

    bool MakeResponse()
    {
        if (_targetfile == "./wwwroot/favicon.ico")
        {
            LOG(LogLevel::DEBUG) << "用户请求: " << _targetfile << "忽略它";
            return false;
        }

        int filesize = 0;
        bool res = Util::ReadFileContent(_targetfile, &_text);
        if (!res)
        {
            _text = "";
            LOG(LogLevel::WARNING) << "client want get : " << _targetfile << " but not found";
            SetCode(404);
            _targetfile = webroot + page_404;
            filesize = Util::FileSize(_targetfile);
            Util::ReadFileContent(_targetfile, &_text);
            std::string suffix = Uri2Suffix(_targetfile);
            SetHeader("Content-Type", suffix);
            SetHeader("Content-Length", std::to_string(filesize));
        }
        else
        {
            LOG(LogLevel::DEBUG) << "读取文件: " << _targetfile;
            SetCode(200);
            filesize = Util::FileSize(_targetfile);
            std::string suffix = Uri2Suffix(_targetfile);
            SetHeader("Conent-Type", suffix);
            SetHeader("Content-Length", std::to_string(filesize));
        }
        return true;
    }

    bool Deserialize(std::string &reqstr)
    {
        return true;
    }

    ~HttpResponse()
    {
    }

public:
    std::string _version;
    int _code;
    std::string _desc;

    std::unordered_map<std::string, std::string> _headers;
    std::string _blankline;
    std::string _text;

    std::string _targetfile;
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
        int n = sock->Recv(&httpreqstr); // 不能保证读到一个完整报文...假设读到了

        if (n > 0)
        {
            std::cout << "##########################" << std::endl;
            std::cout << httpreqstr;
            std::cout << "##########################" << std::endl;
            
            HttpRequest req;
            req.Deserialize(httpreqstr);
            HttpResponse resp;
            resp.SetTargetFile(req.Uri());
            if (resp.MakeResponse())
            {
                std::string response_str = resp.Serialize();
                sock->Send(response_str);
            }
        }

#ifdef DEBUG
        std::string httpreqstr;
        sock->Recv(&httpreqstr);
        std::cout << httpreqstr;

        HttpResponse resp;
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
