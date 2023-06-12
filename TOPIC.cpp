#include "Command.hpp"

void Command::TOPIC()
{
	if (this->_params.size() < 1)
	{
		sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "INVITE"));
		return;
	}// 채널이 없을때
	else if (_server->getChannel(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}// 채널에 존재하지않을때
	else if (_sender->isJoined(this->_params[0]) == false)
	{
		sendReply(_sender->getSocket(), ERR_NOTONCHANNEL(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	} // op만 할 수 있고 +  op권한이 없을때
	else if ((_server->getChannel(this->_params[0])->getMode() & MODE_T) && _server->getChannel(this->_params[0])->isOperator(_sender) == false)
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
