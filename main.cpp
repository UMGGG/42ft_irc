#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"

int main(int argc, char** argv)
{
    try
    {
        if (argc != 3)
            throw std::runtime_error("usage: ./ircserv <port> <password>");

        std::string password = argv[2];
        std::string port_str = argv[1];
        for(size_t i = 0; i < port_str.size() ; i++)
        {
            if (std::isdigit(port_str[i]) == false)
                throw std::runtime_error("Port numbers can only contain numbers.");
        }
        int port = atoi(argv[1]);
        if (port > 65535)
            throw std::runtime_error("Port numbers cannot be greater than 65535.");

        Server server(password);
        server.startServer(port);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
