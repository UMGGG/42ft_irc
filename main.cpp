#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    std::string password = argv[2];
    std::string port_str = argv[1];
    for(size_t i = 0; i < port_str.size() ; i++)
    {
        if (std::isdigit(port_str[i]) == false)
        {
            std::cout << "Port numbers can only contain numbers." << std::endl;
            return 1;
        }
    }
    int port = atoi(argv[1]); // 에러 처리 필요??
    if (port > 65535)
    {
        std::cout << "Port numbers cannot be greater than 65535." << std::endl;
        return 1;
    }

    Server server(password);
    server.startServer(port);

    return 0;
}
