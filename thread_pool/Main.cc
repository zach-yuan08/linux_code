#include "Log.hpp"
#include "ThreadPool.hpp"
#include <memory>

using namespace LogModule;

int main()
{
    // Enable_Console_Log_Strategy();

    // sleep(100);
    // tp->Stop();


    // Enable_Console_Log_Strategy();
    // LOG(LogLevel::DEBUG) << "hello world" << 3.141;
    // LOG(LogLevel::DEBUG) << "hello world" << 3.142;

    // Enable_File_Log_Strategy();
    // LOG(LogLevel::DEBUG) << "hello world" << 3.143;
    // LOG(LogLevel::DEBUG) << "hello world" << 3.144;



    std::unique_ptr<LogStrategy> strategy = std::make_unique<ConsoleLogStrategy>(); // C++14
    // std::unique_ptr<LogStrategy> strategy = std::make_unique<FileLogStrategy>(); // C++14
    strategy->SyncLog("hello log!");
    logger(LogLevel::DEBUG, "main.cc", 10) << "hello world," << 3.14 << " " << 8899 << "aaaa";
    logger(LogLevel::DEBUG, "main.cc", 10) << "hello world";
    logger(LogLevel::DEBUG, "main.cc", 10) << "hello world";
    logger(LogLevel::DEBUG, "main.cc", 10) << "hello world";

    return 0;
}