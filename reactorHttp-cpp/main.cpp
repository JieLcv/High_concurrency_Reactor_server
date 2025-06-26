#include"channel.h"
#include<stdlib.h>
#include<unistd.h>
#include"tcpServer.h"
#include<signal.h>

int main(int argc, char* argv[])
{
    if (sigignore(SIGPIPE) < 0)
    {
        perror("sigpipe ignore error");
    }
    if (argc < 3)
    {
        printf("./a.out port path");
        return -1;
    }
    //切换工作路径
    chdir(argv[2]);
    unsigned int port = atoi(argv[1]);
    TcpServer* server = new TcpServer(port, 4);
    server->run();
    return 0;
}