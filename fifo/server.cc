// #include <iostream>
// #include <string>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include "comm.hpp"

#include "comm.hpp"

int main()
{
    // umask(0);                        // 设置权限掩码
    // int n = mkfifo(FIFO_FILE, 0666); // 创建管道文件，并设置权限
    // if (n < 0)
    // {
    //     std::cout << "mkfile error" << std::endl;
    //     return 1;
    // }
    // std::cout << "mkfifo sucess" << std::endl;

    // // 打开管道文件
    // int fd = open(FIFO_FILE, O_RDONLY);
    // if (fd < 0)
    // {
    //     std::cerr << "open fifo error" << std::endl;
    //     return 2;
    // }
    // std::cout << "open fifo success" << std::endl;

    // // 读文件
    // while (true)
    // {
    //     char buffer[1024];
    //     int number = read(fd, buffer, sizeof(buffer) - 1);
    //     if (number > 0)
    //     {
    //         buffer[number] = 0;                               // 第number个位置充当\0
    //         std::cout << "Clent Say#" << buffer << std::endl; // 打印读到的内容
    //     }
    //     else if(number == 0)
    //     {
    //         std::cout << "client quit!me too" << number << std::endl;
    //         break;
    //     }
    //     else
    //     {
    //         std::cerr << "read error" << std::endl;
    //         break;
    //     }
    // }

    // // 关闭文件
    // close(fd);

    // // 删除管道
    // n = unlink(FIFO_FILE);
    // if (n == 0)
    // {
    //     std::cout << "remove fifo success" << std::endl;
    // }
    // else
    // {
    //     std::cout << "remove fifo failed" << std::endl;
    // }
    // return 0;


    // 创建管道文件
    NamedFifo fifo("/", FILENAME);

    // 文件操作
    FileOper readerfile(PATH, FILENAME);
    readerfile.OpenForRead();
    readerfile.Read();
    readerfile.Close();

}