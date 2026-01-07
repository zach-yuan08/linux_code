#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "./mykill signumber pid" << std::endl;
        return 1;
    }
    int signum = std::stoi(argv[1]); // 将信号转为信号编号
    pid_t target = std ::stoi(argv[2]);

    int n = kill(target, signum);
    if (n == 0)
    {
        std ::cout << "send" << signum << "to" << target << "success." << std::endl;
    }
    return 0;
}