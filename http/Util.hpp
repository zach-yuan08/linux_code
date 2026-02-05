#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "Http.hpp"

class Util
{
public:
    // 获取页面内容
    static bool ReadFileContent(const std::string &filename, std::string *out)
    {
        int filesize = FileSize(filename);
        if (filesize > 0)
        {
            std::ifstream in(filename);
            if (!in.is_open())
                return false;
            out->resize(filesize);
            in.read((char *)(out->c_str()), filesize);
            in.close();
        }
        else
        {
            return false;
        }

        return true;
    }

    // 获取一行
    static bool ReadOneLine(std::string &bigstr, std::string *out, const std::string &sep)
    {
        auto pos = bigstr.find(sep);
        if (pos == std::string::npos)
            return false;

        *out = bigstr.substr(0, pos);
        bigstr.erase(0, pos + sep.size());
        return true;
    }

    // 获取文件大小
    static int FileSize(const std::string &filename)
    {
        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open())
            return -1;
        in.seekg(0, in.end);
        int filesize = in.tellg();
        in.seekg(0, in.beg);
        in.close();
        return filesize;
    }
};
