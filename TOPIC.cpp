#include "Command.hpp"

void Command::TOPIC()
{
	if (this->_params.size() < 1)
	{
		sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "TOPIC"));
		return;
	}// sender is not register
	else if (_sender->getStatus() != CONNECTED)
	{
		sendReply(_sender->getSocket(), ERR_NOTREGISTERED(_server->getName(), _sender->getNick(), "TOPIC"));
		return;
	}// channel is not exist
	else if (_server->getChannel(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	if (this->_trailing.empty() == false)
	{
		// sender is not exist at channel with set topic
		if (_sender->isJoined(this->_params[0]) == false)
		{
			sendReply(_sender->getSocket(), ERR_NOTONCHANNEL(_server->getName(), _sender->getNick(), this->_params[0]));
			return ;
		} // sender is not op AND channel mode is +t
		else if ((_server->getChannel(this->_params[0])->getMode() & MODE_T) && _server->getChannel(this->_params[0])->isOperator(_sender) == false)
		{
			sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED_TOPIC(_server->getName(), _sender->getNick(), this->_params[0]));
			return ;
		}
		_server->getChannel(this->_params[0])->setTopic(this->_trailing);
		_server->getChannel(this->_params[0])->setSetter(_sender->getNick() + "!" + _sender->getUsername() + "@" + _sender->getIP());
		_server->getChannel(this->_params[0])->setTime(time(NULL));
		std::string message = RPL_SETTOPIC(_sender->getNick(), _sender->getUsername(), _sender->getIP(), this->_params[0], this->_trailing);
		_server->getChannel(this->_params[0])->sendReply(message);
		return ;
	}
	else
	{
		if (_server->getChannel(this->_params[0])->getTopic() == "")
		{
			sendReply(_sender->getSocket(), RPL_NOTOPIC(_sender->getServer()->getName(), _sender->getNick(), this->_params[0]));
			return ;
		}
		else
		{
			sendReply(_sender->getSocket(), RPL_TOPIC(_sender->getServer()->getName(), _sender->getNick(), this->_params[0], _server->getChannel(this->_params[0])->getTopic()));
			std::stringstream strTime;
			strTime << _server->getChannel(this->_params[0])->getTime();
			sendReply(_sender->getSocket(), RPL_TOPICWHOTIME(_sender->getServer()->getName(), _sender->getNick(), this->_params[0], _server->getChannel(this->_params[0])->getSetter(), strTime.str()));
			return ;
		}
	}
}
