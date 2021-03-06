#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <iostream>
#include "executor.hpp"

bool set_daemon()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "fork failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (setsid() < 0)
        {
            std::cerr << "setsid failed!" << std::endl;
            return false;
        }

        umask(0);
        chdir("/");
        for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--)
            close (x);
        openlog ("executor_daemon", LOG_PID, LOG_DAEMON);
    }
    else
        exit(EXIT_SUCCESS);
    return true;
}

int main(int argc,char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "argc is not correct!" << std::endl;
        return 0;
    }
    
    if (!set_daemon())
    {
        std::cerr << "set_daemon failed!"  << '\n';
        return -1;
    }
    std::string port{argv[2]};
    Executor executor(argv[1],std::stoi(port));
    executor.init();
    return 0;
}
