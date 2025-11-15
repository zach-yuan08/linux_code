// #include <iostream>
// #include <string>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include "comm.hpp"
// #include <unistd.h>
// #include <fcntl.h>

#include "comm.hpp"

int main()
{
    // // 打开管道文件
    // int fd = open(FIFO_FILE, O_WRONLY);
    // if (fd < 0)
    // {
    //     std::cerr << "open fifo error" << std::endl;
    //     return 2;
    // }
    // std::cout << "open fifo success" << std::endl;

    // // 写文件
    // std::string message;
    // int cnt = 1;
    // pid_t id = getpid();
    // while (true)
    // {
    //     std::cout << "Please Enter#" << std::endl;
    //     std::getline(std::cin, message);
    //     message += (", message number:" + std::to_string(cnt) + ",[" + std::to_string(id) + "]");
    //     write(fd, message.c_str(), message.size()); // c_str方法 就是拿到string中的数据
    // }

    // // 关闭文件
    // close(fd);

    // return 0;

    FileOper writefiler(PATH, FILENAME);
    writefiler.OpenForWrite();
    writefiler.Write();
    writefiler.Close();
}