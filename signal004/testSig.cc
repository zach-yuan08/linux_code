#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <vector>



// void handler(int signum)
// {
//     std::cout << "hello signal:" << signum << std::endl;
//     exit(0);
// }
// int main()
// {
//     struct sigaction act, oact;
//     act.sa_handler = handler;

//     sigaction(SIGINT, &act, &oact);

//     while (true)
//     {
//         std::cout << "hello world" << std :: endl;
//         sleep(1);
//     }

//     return 0;
// }