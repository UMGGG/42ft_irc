#include "Command.hpp"

void Command::TOPIC()
{
	std::cout << this->_trailing << std::endl;
	//채널에 존재하지않을때, 채널이 없을때
	if (_server->getChannel(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	} // op만 할 수 있고, op권한이 없을때(MODE에서 k옵션 들어있는지 불러와서 확인해야함)
	else if (_server->getChannel(this->_params[0])->isOperator(_sender) == false)
	{
		sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED_TOPIC(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	if (this->_trailing.empty() == false)
	{
		_server->getChannel(this->_params[0])->setTopic(this->_trailing);
		_server->getChannel(this->_params[0])->setSetter(_sender->getNick() + "!" + _sender->getUsername() + "@" + _sender->getIP());
		_server->getChannel(this->_params[0])->setTime(time(NULL));
		std::string message = RPL_SETTOPIC(_sender->getNick(), _sender->getUsername(), _sender->getIP(), this->_params[0], this->_trailing);
		_server->getChannel(this->_params[0])->sendReply(message);
		return ;
	}
	else //topic을 불러오는건 client에서 관리되는듯??
	{
		if (_server->getChannel(this->_params[0])->getTopic() == "")
		{
			std::string message = RPL_NOTOPIC(_sender->getServer()->getName(), _sender->getNick(), this->_params[0]);
			_server->getChannel(this->_params[0])->sendReply(message);
			return ;
		}
	}
}
