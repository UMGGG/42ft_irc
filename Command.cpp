#include "Command.hpp"

/*Command::Command(std::string pre, std::string com, std::vector<std::string> par, std::string tra)
{
	this->prefix = pre;
	this->command = com;
	this->params = par;
	this->trailer = tra;
}*/

void Command::send_single(int fd, std::string message)
{
	int ret;

	ret = send(fd, message.c_str(), message.size(), MSG_DONTWAIT);
	if (ret == -1)
		std::cerr << "Error: " << message.c_str() << std::endl;
	std::cout << "Send to client " << fd << std::endl;
	std::cout << message << std::endl;
}

void Command::send_all(std::vector<int> fds, std::string message)
{
	for (size_t i = 0 ; i < fds.size() ; i++)
		send_single(fds[i], message);
}

Command::Command(Server *server, User *sender)
: _server(server), _sender(sender), _command(""), _trailing("")
{
    std::string message = _sender->getMessage();
    std::stringstream stream;
    std::string buf;

    message.erase(message.find("\r\n")); // crlf 제거

    if (message.find(':') != std::string::npos) // _trailing (include ':')
    {
        _trailing.append(message.substr(message.find(':')));
        message.erase(message.find(':')); // message에서 _trailing 제거
    }

    stream.str(message);

    std::getline(stream, buf, ' ');
    _command.append(buf); // _command
    buf.clear();

    while(std::getline(stream, buf, ' ')) // _params
    {
        _params.push_back(buf);
        buf.clear();
    }
}

void Command::testPrint()
{
    if (_command.empty())
        std::cout << "Command :\tNone" << std::endl;
    else
        std::cout << "Command :\t" << _command << std::endl;
    for (std::vector<std::string>::iterator it = _params.begin(); it != _params.end(); ++it)
        std::cout << "Parameter :\t" << *it << std::endl;
    if (_trailing.empty())
        std::cout << "Trailing :\tNone" << std::endl;
    else
        std::cout << "Trailing :\t" << _trailing << std::endl;
    std::cout << "------------" << std::endl;
}
