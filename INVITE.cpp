#include "Command.hpp"

void Command::INVITE()
{
	if (this->_params.size() < 1)
	{
		sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "INVITE"));
		return;
	}// channel not exist
	else if (_server->getChannel(this->_params[1]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[1]));
		return ;
	}// target user is not exist at server
	else if (_server->getUser(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}// sender is not exist at channel
	else if (_sender->isJoined(this->_params[1]) == false)
	{
		sendReply(_sender->getSocket(), ERR_NOTONCHANNEL(_server->getName(), _sender->getNick(), this->_params[1]));
		return ;
	} // target user is already at channel
	else if (_server->getChannel(this->_params[1])->getUser(this->_params[0]) != NULL)
	{
		sendReply(_sender->getSocket(), ERR_USERONCHANNEL(_sender->getNick(), _server->getName(), this->_params[1], this->_params[0]));
		return ;
	}
	sendReply(_sender->getSocket(), RPL_INVITING(_sender->getNick(), _server->getName(), this->_params[0], this->_params[1]));
	sendReply(_server->getUser(this->_params[0])->getSocket(), RPL_INVITED(_sender->getNick(), _sender->getUsername(), _sender->getIP(), this->_params[0], this->_params[1]));
	_server->getUser(this->_params[0])->isInvited(_server->getChannel(this->_params[1])); // 있다면 삭제되고 없다면 아무일도 안일어남
	_server->getUser(this->_params[0])->addInvited(_server->getChannel(this->_params[1]));
}
