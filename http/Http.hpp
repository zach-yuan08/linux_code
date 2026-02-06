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
        ss >> _method >> _uri >> _version; // std::stringstream 的 >> 提取运算符遵循空白字符分割规则
    }

    bool Deserialize(std::string &reqstr)
    {
        // 1. 提取请求中的请求行
        std::string reqline;
        bool res = Util::ReadOneLine(reqstr, &reqline, glinespace); // 获取请求行
        LOG(LogLevel::DEBUG) << reqline;

        // 2. 对请求行反序列化
        ParseReqLine(reqline);

        if (_uri == "/") // 如果访问的是根目录，返回首页
            _uri = webroot + _uri + homepage;
        else
            _uri = webroot + _uri;

        LOG(LogLevel::DEBUG) << "_method:" << _method;
        LOG(LogLevel::DEBUG) << "_uri:" << _uri;
        LOG(LogLevel::DEBUG) << "_version:" << _version;
        const std::string temp = "?";
        auto pos = _uri.find(temp);
        if (pos == std::string::npos)
        {
            return true;
        }
        // _uri: ./wwwroot/login
        // username=zhangsan&password=123456
        _args = _uri.substr(pos + temp.size());
        _uri = _uri.substr(0, pos);
        _is_interact = true;

        // ./wwwroot/XXX.YYY
        return true;
    }

    std::string Uri() { return _uri; }
    bool isInteract() { return _is_interact; }
    std::string Args() { return _args; }

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

    std::string _args;
    bool _is_interact;
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
        case 301:
            _desc = "Moved Permanently";
            break;
        case 302:
            _desc = "See Other";
            break;
        case 404:
            _desc = "NotFound";
            break;
        default:
            break;
        }
    }

    // 设置报头
    void SetHeader(const std::string &key, const std::string &value)
    {
        auto iter = _headers.find(key);
        if (iter != _headers.end())
            return;
        _headers.insert(std::make_pair(key, value));
    }

    // 提取文件后缀，返回对应文件类型
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
        if (_targetfile == "./wwwroot/redir_test")
        {
            SetCode(301);
            SetHeader("Location", "https://www.qq.com/");
            return true;
        }

        int filesize = 0;
        bool res = Util::ReadFileContent(_targetfile, &_text);
        if (!res)
        {
            // 1. 清空响应体缓冲区，避免残留旧数据
            _text = "";
            // 2. 打印警告日志：记录客户端请求的文件不存在
            LOG(LogLevel::WARNING) << "client want get : " << _targetfile << " but not found";
            // 3. 设置HTTP状态码为404，标识资源未找到
            SetCode(404);
            // 4. 重新指定目标文件为服务器内置的404页面
            _targetfile = webroot + page_404;
            // 5. 获取404页面的文件大小，用于设置响应头
            filesize = Util::FileSize(_targetfile);
            // 6. 读取404页面的HTML内容，写入响应体
            Util::ReadFileContent(_targetfile, &_text);
            // 7. 根据文件后缀解析MIME类型，用于告知浏览器数据格式
            std::string suffix = Uri2Suffix(_targetfile);
            // 8. 设置响应头：告诉浏览器响应体的内容类型
            SetHeader("Content-Type", suffix);
            // 9. 设置响应头：告诉浏览器响应体的字节长度
            SetHeader("Content-Length", std::to_string(filesize));

            // SetCode(302);
            // SetHeader("Location", "http://8.137.19.140:8080/404.html");
            // return true;
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

    void SetText(const std::string &t)
    {
        _text = t;
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

using http_func_t = std::function<void(HttpRequest &req, HttpResponse &resp)>;

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
            HttpResponse resp;
            req.Deserialize(httpreqstr);
            if (req.isInteract())
            {
                // _uri: ./wwwroot/login
                if (_route.find(req.Uri()) == _route.end())
                {
                    // SetCode(302)
                }
                else
                {
                    _route[req.Uri()](req, resp);
                    std::string response_str = resp.Serialize();
                    sock->Send(response_str);
                }
            }
            else
            {
                // 我这里有一个疑惑：req.Uri()已经能获取目标网页，为什么还要调用SetTargetFile？
                // 答案是：能实现请求与应答的解耦
                resp.SetTargetFile(req.Uri()); // 判断要获取哪个网页

                if (resp.MakeResponse()) // 构建响应报文
                {
                    std::string response_str = resp.Serialize();
                    sock->Send(response_str);
                }
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

    void RegisterService(const std::string name, http_func_t h)
    {
        std::string key = webroot + name; // ./wwwroot/login
        auto iter = _route.find(key);
        if (iter == _route.end())
        {
            _route.insert(std::make_pair(key, h));
        }
    }

    ~Http()
    {
    }

private:
    std::unique_ptr<TcpServer> tsvrp;
    std::unordered_map<std::string, http_func_t> _route;
};
