#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>
#include <functional>
#include "Socket.hpp"

using namespace SocketModule;

// 约定好各个字段的含义，本质上就是约定好协议。
// client -> server
class Request
{
public:
    Request()
    {
    }

    Request(int x, int y, char oper) : _x(x), _y(y), _oper(oper)
    {
    }

    std::string Serialize()
    {
        std::string s;
        Json::Value root;
        root["x"] = _x;
        root["y"] = _y;
        root["oper"] = _oper;

        Json::FastWriter writer;
        s = writer.write(root);
        return s;
    }

    bool Deserialize(std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        bool ok = reader.parse(in, root);
        if (ok)
        {
            _x = root["x"].asInt();
            _y = root["y"].asInt();
            _oper = root["oper"].asInt();
        }
        return ok;
    }

    ~Request()
    {
    }
    int X() { return _x; }
    int Y() { return _y; }
    char Oper() { return _oper; }

private:
    int _x;
    int _y;
    char _oper;
};

// server -> client
class Response
{
public:
    Response()
    {
    }

    Response(int result, int code) : _result(result), _code(code)
    {
    }

    std::string Serialize()
    {
        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;

        Json::FastWriter writer;
        return writer.write(root);
    }

    bool Deserialize(std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        bool ok = reader.parse(in, root);
        if (ok)
        {
            _result = root["result"].asInt();
            _code = root["code"].asInt();
        }
        return ok;
    }

    void SetResult(int res)
    {
        _result = res;
    }

    void SetCode(int code)
    {
        _code = code;
    }

    void ShowResult()
    {
        std::cout << "计算结果是：" << _result << "[" << _code << "]" << std::endl;
    }

    ~Response()
    {
    }

private:
    int _result;
    int _code; // 表示运算结果的退出码
};

// 协议（基于TCP）需要解决的两个问题：
// 1. request和response 必须得有序列化和反序列化功能
// 2. 读取的时候要读取到完整的请求

const std::string sep = "\r\n";
using func_t = std::function<Response(Request &req)>;

class Protocol
{
public:
    Protocol()
    {
    }

    Protocol(func_t func) : _func(func)
    {
    }

    // 应用层封装报文（添加报头）
    std::string Encode(const std::string &jsonstr)
    {
        std::string len = std::to_string(jsonstr.size());
        return len + sep + jsonstr + sep;
    }

    // 1. 判读报文完整性
    // 2. 如果包含至少一个完整请求，提取他，并从接收缓冲区中移除
    bool DeCode(std::string &buffer, std::string *package)
    {
        ssize_t pos = buffer.find(sep);
        if (pos == std::string::npos)
            return false;

        std::string package_len_str = buffer.substr(0, pos);
        int package_len_int = std::stoi(package_len_str);
        int target_len = package_len_str.size() + package_len_int + 2 * sep.size();
        if (buffer.size() < target_len)
            return false;

        *package = buffer.substr(pos + sep.size(), package_len_int);
        buffer.erase(0, target_len);

        return true;
    }

    void GetRequest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {
        std::string buffer_queue;
        while (true)
        {
            int n = sock->Recv(&buffer_queue);
            if (n > 0)
            {
                std::cout << "----------request_buffer------------" << std::endl;
                std::cout << buffer_queue << std::endl;
                std::cout << "------------------------------------" << std::endl;

                // 1. 拿完整报文
                std::string json_package;

                // 2. 做反序列化
                while (DeCode(buffer_queue, &json_package))
                {
                    std::cout << "-----------request_json-------------" << std::endl;
                    std::cout << json_package << std::endl;
                    std::cout << "------------------------------------" << std::endl;

                    std::cout << "-----------request_buffer-----------" << std::endl;
                    std::cout << buffer_queue << std::endl;
                    std::cout << "------------------------------------" << std::endl;
                    Request req;
                    bool ok = req.Deserialize(json_package);
                    if (!ok)
                        continue;
                    // 3. 完成计算功能（通过req得到resp）
                    Response resp = _func(req);

                    // 4. 序列化
                    std::string json_str = resp.Serialize();

                    // 5. 加报头
                    std::string send_str = Encode(json_str);

                    // 6. 发送
                    sock->Send(send_str);
                }
            }
            else if (n == 0)
            {
                LOG(LogLevel::INFO) << "client:" << client.StringAddr() << "Quit!";
                break;
            }
            else
            {
                LOG(LogLevel::WARNING) << "client:" << client.StringAddr() << ", recv error";
                break;
            }
        }
        sock->Close();
    }

    bool GetResponse(std::shared_ptr<Socket> &client, std::string &resp_buff, Response *resp)
    {
        std::string resp_str;
        while (true)
        {
            int n = client->Recv(&resp_str);
            if (n > 0)
            {
                std::cout << "------------resp_buffer-------------" << std::endl;
                std::cout << resp_buff << std::endl;
                std::cout << "------------------------------------" << std::endl;

                // 1. 拿完整报文
                std::string json_package;
                while (DeCode(resp_str, &json_package))
                {

                    std::cout << "-----------response json------------" << std::endl;
                    std::cout << json_package << std::endl;
                    std::cout << "------------------------------------" << std::endl;

                    std::cout << "-----------response json------------" << std::endl;
                    std::cout << resp_buff << std::endl;
                    std::cout << "------------------------------------" << std::endl;

                    // 2. 做反序列化
                    resp->Deserialize(json_package);
                }
                return true;
            }
            else if (n == 0)
            {
                std::cout << "server quit " << std::endl;
                return false;
            }
            else
            {
                std::cout << "recv error" << std::endl;
                return false;
            }
        }
    }

    std::string BuildRequestString(int x, int y, char oper)
    {
        // 1. 构建请求
        Request req(x, y, oper);

        // 2. 序列化
        std::string josn_req = req.Serialize();
        // 2.1 debug
        std::cout << "----------json_req string-----------" << std::endl;
        std::cout << josn_req << std::endl;
        std::cout << "------------------------------------" << std::endl;

        // 3. 加报头
        return Encode(josn_req);
    }

    ~Protocol()
    {
    }

private:
    // Request _req;
    // Response _resp;
    func_t _func;
};